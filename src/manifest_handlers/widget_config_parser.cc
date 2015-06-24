// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "manifest_handlers/widget_config_parser.h"

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

bool WidgetConfigParser::ParseManifest(const boost::filesystem::path& path) {
  widget_path_ = path.parent_path();
  if (!parser_->ParseManifest(path))
    return false;

  // TODO(t.iwanek): implement additional w3c checks!

  return true;
}

}  // namespace parse
}  // namespace wgt
