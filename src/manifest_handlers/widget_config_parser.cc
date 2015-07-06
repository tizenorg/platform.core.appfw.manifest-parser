// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "manifest_handlers/widget_config_parser.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>

#include <xdgmime.h>

#include <algorithm>
#include <string>
#include <map>
#include <vector>

#include "manifest_handlers/app_control_handler.h"
#include "manifest_handlers/application_icons_handler.h"
#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_handlers/appwidget_handler.h"
#include "manifest_handlers/category_handler.h"
#include "manifest_handlers/content_handler.h"
#include "manifest_handlers/ime_handler.h"
#include "manifest_handlers/metadata_handler.h"
#include "manifest_handlers/navigation_handler.h"
#include "manifest_handlers/permissions_handler.h"
#include "manifest_handlers/setting_handler.h"
#include "manifest_handlers/splash_screen_handler.h"
#include "manifest_handlers/tizen_application_handler.h"
#include "manifest_handlers/warp_handler.h"
#include "manifest_handlers/widget_handler.h"
#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/manifest_constants.h"
#include "utils/logging.h"

namespace ba = boost::algorithm;
namespace bf = boost::filesystem;
namespace bs = boost::system;

namespace {

const char kLocaleDirectory[] = "locales";

const char* kDefaultStartFiles[] = {
  "index.htm",
  "index.html",
  "index.svg",
  "index.xhtml",
  "index.xht"
};

const char* kDefaultIconFiles[] = {
  "icon.svg",
  "icon.ico",
  "icon.png",
  "icon.gif",
  "icon.jpg"
};

const std::map<std::string, std::string> kFileIdenticationTable = {
  {".html",  "text/html"},
  {".htm",   "text/html"},
  {".css",   "text/css"},
  {".js",    "application/javascript"},
  {".xml",   "application/xml"},
  {".txt",   "text/plain"},
  {".wav",   "audio/x-wav"},
  {".xhtml", "application/xhtml+xml"},
  {".xht",   "application/xhtml+xml"},
  {".gif",   "image/gif"},
  {".png",   "image/png"},
  {".ico",   "image/vnd.microsoft.icon"},
  {".svg",   "image/svg+xml"},
  {".jpg",   "image/jpeg"},
  {".mp3",   "audio/mpeg"}
};

const char* kIconsSupportedMimeTypes[] = {
  "image/gif",
  "image/png",
  "image/vnd.microsoft.icon",
  "image/svg+xml",
  "image/jpeg"
};

// http://www.w3.org/TR/widgets/#rule-for-identifying-the-media-type-of-a-file-0
bool IsIconMimeTypeSupported(const bf::path& icon_path) {
  std::string ext = icon_path.extension().string();
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  std::string mime_type;
  if (!ext.empty() && std::all_of(++ext.begin(), ext.end(), ::isalpha)) {
    auto iter = kFileIdenticationTable.find(ext);
    if (iter != kFileIdenticationTable.end()) {
      mime_type = iter->second;
    }
  }
  if (mime_type.empty()) {
    // sniff the mime type
    mime_type =
        xdg_mime_get_mime_type_for_file(icon_path.string().c_str(), nullptr);
  }
  for (auto mime : kIconsSupportedMimeTypes) {
    if (mime_type == mime)
      return true;
  }
  return false;
}

enum class FindResult {
  OK,
  NUL,
  ERROR
};

// http://www.w3.org/TR/widgets/#rule-for-finding-a-file-within-a-widget-package-0  // NOLINT
FindResult FindFileWithinWidget(const bf::path& widget_path,
    const std::string& content, bf::path* file = nullptr) {
  std::string content_value = content;
  if (content_value.empty())
    return FindResult::ERROR;
  if (content_value[0] == '/') {
    content_value = content_value.substr(1);
  }
  // Steps 4. && 5.
  std::vector<std::string> path_components;
  ba::split(path_components, content_value, ba::is_any_of("/"));
  if (path_components.size() >= 1 && path_components[0] == kLocaleDirectory) {
    if (path_components.size() == 1) {
      return FindResult::NUL;
    }

    // TODO(t.iwanek): validate language tag

    path_components.erase(path_components.begin(), ++++path_components.begin());
    content_value = ba::join(path_components, "/");
  }
  // for each locale in widget
  // TODO(t.iwanek): this algorithm should accept input of preferred locale list
  bs::error_code error;
  bf::path locale_directory = widget_path / kLocaleDirectory;
  if (bf::exists(locale_directory, error)) {
    for (auto iter = bf::directory_iterator(locale_directory);
         iter != bf::directory_iterator(); ++iter) {
      const bf::path& path = *iter;

      // TODO(t.iwanek): validate language tag

      bf::path candidate = path / content_value;
      if (bf::exists(candidate, error)) {
        if (bf::is_directory(candidate, error)) {
          return FindResult::ERROR;
        }
        if (file)
          *file = candidate;
        return FindResult::OK;
      }
    }
  }

  // default locale
  bf::path root_candidate = widget_path / content_value;
  if (bf::exists(root_candidate, error)) {
    if (bf::is_directory(root_candidate, error)) {
      return FindResult::ERROR;
    }
    if (file)
      *file = root_candidate;
    return FindResult::OK;
  }

  return FindResult::NUL;
}

bool CheckW3CContentSrcExits(const bf::path& widget_path,
                             const std::string& content) {
  if (!content.empty()) {
    if (FindFileWithinWidget(widget_path, content) == FindResult::OK) {
      LOG(INFO) << "Start file is: " << content;
      return true;
    }
  }
  return false;
}

// http://www.w3.org/TR/widgets/#step-8-locate-the-start-file
bool CheckStartFileInWidget(const bf::path& widget_path) {
  for (auto& file : kDefaultStartFiles) {
    if (FindFileWithinWidget(widget_path, file) == FindResult::OK) {
      LOG(INFO) << "Start file is: " << file;
      return true;
    }
  }
  LOG(ERROR) << "No valid start file found";
  return false;
}

}  // namespace

namespace wgt {
namespace parse {

WidgetConfigParser::WidgetConfigParser() {
  std::vector<parser::ManifestHandler*> handlers = {
    new AppControlHandler,
    new ApplicationIconsHandler,
    new AppWidgetHandler,
    new CategoryHandler,
    new ContentHandler,
    new ImeHandler,
    new MetaDataHandler,
    new NavigationHandler,
    new PermissionsHandler,
    new SettingHandler,
    new SplashScreenHandler,
    new TizenApplicationHandler,
    new WarpHandler,
    new WidgetHandler
  };

  std::unique_ptr<parser::ManifestHandlerRegistry> registry(
      new parser::ManifestHandlerRegistry(handlers));

  parser_.reset(new parser::ManifestParser(std::move(registry)));
}

std::shared_ptr<const parser::ManifestData> WidgetConfigParser::GetManifestData(
    const std::string& key) {
  // TODO(t.iwanek): replace this method with required getters
  // do not force client to use keyys from application_manifest_constants
  return parser_->GetManifestData(key);
}

const std::string& WidgetConfigParser::GetErrorMessage() const {
  if (!error_.empty())
    return error_;
  return parser_->GetErrorMessage();
}

bool WidgetConfigParser::CheckStartFile() {
  std::shared_ptr<const ContentInfo> content_info =
      std::static_pointer_cast<const ContentInfo>(parser_->GetManifestData(
          application_widget_keys::kTizenContentKey));
  if (content_info) {
    std::string content = content_info->src();
    if (content_info->is_tizen_content()) {
      // external url in tizen:content is outside of w3c p&c spec
      return true;
    }

    if (CheckW3CContentSrcExits(widget_path_, content)) {
      // Content is valid
      return true;
    } else {
      // Remove content as it is invalid
      parser_->EraseManifestData(application_widget_keys::kTizenContentKey);
    }
  }

  if (!CheckStartFileInWidget(widget_path_)) {
    error_ = "Could not find valid start file";
    return false;
  }
  return true;
}

bool WidgetConfigParser::CheckWidgetIcons() {
  std::shared_ptr<ApplicationIconsInfo> icons_info =
      std::static_pointer_cast<ApplicationIconsInfo>(
          parser_->AccessManifestData(application_manifest_keys::kIconsKey));
  if (!icons_info) {
    error_ = "Failed to get icon info";
    return false;
  }
  ApplicationIconsInfo icons;
  // custom icons
  for (auto& icon : icons_info->get_icon_paths()) {
    bf::path result;
    if (FindFileWithinWidget(widget_path_, icon, &result) == FindResult::OK) {
      if (!IsIconMimeTypeSupported(result)) {
        LOG(WARNING) << "Unsupported icon: " << result;
        continue;
      }
      std::string relative =
          result.string().substr(widget_path_.string().size() + 1);
      icons.add_icon_path(relative);
    }
  }
  // default icons
  for (auto& icon : kDefaultIconFiles) {
    bf::path result;
    if (FindFileWithinWidget(widget_path_, icon, &result) == FindResult::OK) {
      if (!IsIconMimeTypeSupported(result)) {
        LOG(WARNING) << "Unsupported icon: " << result;
        continue;
      }
      std::string relative =
          result.string().substr(widget_path_.string().size() + 1);
      icons.add_icon_path(relative);
    }
  }
  for (auto& icon : icons.get_icon_paths()) {
    LOG(DEBUG) << "Valid icon: " << icon;
  }
  *icons_info = icons;
  return true;
}

bool WidgetConfigParser::ParseManifest(const boost::filesystem::path& path) {
  widget_path_ = path.parent_path();
  if (!parser_->ParseManifest(path))
    return false;

  if (!CheckStartFile())
    return false;

  if (!CheckWidgetIcons())
    return false;

  return true;
}

}  // namespace parse
}  // namespace wgt
