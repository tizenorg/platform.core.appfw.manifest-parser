// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/tpk_config_parser.h"

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/utils/iri_util.h"
#include "manifest_parser/utils/logging.h"
#include "tpk_manifest_handlers/account_handler.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "tpk_manifest_handlers/author_handler.h"
#include "tpk_manifest_handlers/description_handler.h"
#include "tpk_manifest_handlers/package_handler.h"
#include "tpk_manifest_handlers/privileges_handler.h"
#include "tpk_manifest_handlers/profile_handler.h"
#include "tpk_manifest_handlers/service_application_handler.h"
#include "tpk_manifest_handlers/shortcut_handler.h"
#include "tpk_manifest_handlers/ui_application_handler.h"
#include "tpk_manifest_handlers/widget_application_handler.h"
#include "tpk_manifest_handlers/watch_application_handler.h"

namespace bf = boost::filesystem;

namespace {

const char kLocaleDirectory[] = "locales";

}  // namespace

namespace tpk {
namespace parse {

TPKConfigParser::TPKConfigParser() {
  std::vector<std::shared_ptr<parser::ManifestHandler>> handlers = {
    std::make_shared<AccountHandler>(),
    std::make_shared<AuthorHandler>(),
    std::make_shared<DescriptionHandler>(),
    std::make_shared<PackageHandler>(),
    std::make_shared<PrivilegesHandler>(),
    std::make_shared<ProfileHandler>(),
    std::make_shared<WidgetApplicationHandler>(),
    std::make_shared<WatchApplicationHandler>(),
    std::make_shared<ServiceApplicationHandler>(),
    std::make_shared<UIApplicationHandler>(),
    std::make_shared<ShortcutHandler>()
  };

  std::unique_ptr<parser::ManifestHandlerRegistry> registry(
      new parser::ManifestHandlerRegistry(handlers));

  parser_.reset(new parser::ManifestParser(std::move(registry)));
}

std::shared_ptr<const parser::ManifestData> TPKConfigParser::GetManifestData(
    const std::string& key) {
  return parser_->GetManifestData(key);
}

const std::string& TPKConfigParser::GetErrorMessage() const {
  if (!error_.empty())
    return error_;
  return parser_->GetErrorMessage();
}

bool TPKConfigParser::ParseManifest(const boost::filesystem::path& path) {
  if (!parser_->ParseManifest(path))
    return false;

  return true;
}

void ApplicationIconsInfo::AddIcon(const ApplicationIcon& new_icon) {
  // Eliminate duplicates, keep order
  if (std::find_if(icons_.begin(), icons_.end(),
      [&new_icon](const ApplicationIcon& icon) {
        return icon.path() == new_icon.path();
      })
      != icons_.end()) {
    return;
  }
  icons_.push_back(new_icon);
}

void ApplicationSplashScreenInfo::AddSplashScreen(
    const ApplicationSplashScreen& new_splashscreen) {
  // Eliminate duplicates, keep order
  if (std::find_if(splashscreens_.begin(), splashscreens_.end(),
      [&new_splashscreen](const ApplicationSplashScreen& splashscreen) {
        return splashscreen.src() == new_splashscreen.src();
      })
      != splashscreens_.end()) {
    return;
  }
  splashscreens_.push_back(new_splashscreen);
}

}  // namespace parse
}  // namespace tpk
