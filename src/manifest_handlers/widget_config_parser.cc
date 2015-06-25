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

#include <algorithm>
#include <string>
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

// http://www.w3.org/TR/widgets/#step-8-locate-the-start-file
bool CheckStartFileInWidget(const bf::path& widget_path,
                            const std::string& content) {
  if (!content.empty()) {
    if (FindFileWithinWidget(widget_path, content) == FindResult::OK) {
      LOG(INFO) << "Start file is: " << content;
      return true;
    }
  }
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
  std::string content;
  std::shared_ptr<const ContentInfo> content_info =
      std::static_pointer_cast<const ContentInfo>(parser_->GetManifestData(
          application_widget_keys::kTizenContentKey));
  if (content_info) {
    content = content_info->src();
    if (content_info->is_tizen_content()) {
      // external url in tizen:content is outside of w3c p&c spec
      return true;
    }
  }

  if (!CheckStartFileInWidget(widget_path_, content)) {
    error_ = "Could not find valid start file";
    return false;
  }
  return true;
}

bool WidgetConfigParser::ParseManifest(const boost::filesystem::path& path) {
  widget_path_ = path.parent_path();
  if (!parser_->ParseManifest(path))
    return false;

  if (!CheckStartFile())
    return false;

  return true;
}

}  // namespace parse
}  // namespace wgt
