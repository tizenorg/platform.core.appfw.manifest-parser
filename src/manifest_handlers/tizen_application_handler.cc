// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/tizen_application_handler.h"

#include <cassert>
#include <map>
#include <utility>

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_parser/manifest_util.h"
#include "manifest_handlers/platform_version.h"
#include "manifest_parser/values.h"
#include "utils/version_number.h"

namespace wgt {
namespace parse {

namespace {
const utils::VersionNumber kMinimumAPIVersion("2.2.1");
const utils::VersionNumber kLaunchModeRequiredVersion("2.4");
}  // namespace

namespace keys = wgt::application_widget_keys;

TizenApplicationInfo::TizenApplicationInfo() {}

TizenApplicationInfo::~TizenApplicationInfo() {}

TizenApplicationHandler::TizenApplicationHandler() {}

TizenApplicationHandler::~TizenApplicationHandler() {}

bool TizenApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<TizenApplicationInfo> app_info(new TizenApplicationInfo);
  parser::Value* app_value = nullptr;
  manifest.Get(keys::kTizenApplicationKey, &app_value);
  // Find an application element with tizen namespace
  parser::DictionaryValue* app_dict;
  std::string value;
  bool found = false;
  if (app_value && app_value->IsType(parser::Value::TYPE_DICTIONARY)) {
    app_value->GetAsDictionary(&app_dict);
    found = app_dict->GetString(keys::kNamespaceKey, &value);
    found = found && (value == keys::kTizenNamespacePrefix);
  } else if (app_value && app_value->IsType(parser::Value::TYPE_LIST)) {
    parser::ListValue* list;
    app_value->GetAsList(&list);
    for (parser::ListValue::iterator it = list->begin();
         it != list->end(); ++it) {
      (*it)->GetAsDictionary(&app_dict);
      app_dict->GetString(keys::kNamespaceKey, &value);
      bool is_tizen = (value == keys::kTizenNamespacePrefix);
      if (is_tizen) {
        if (found) {
          *error = "There should be no more than one tizen:application element";
          return false;
        }
        found = true;
      }
    }
  }

  if (!found) {
    *error = "Cannot find application element with tizen namespace "
             "or the tizen namespace prefix is incorrect.\n";
    return false;
  }
  if (app_dict->GetString(keys::kTizenApplicationIdKey, &value))
    app_info->set_id(value);
  if (app_dict->GetString(keys::kTizenApplicationPackageKey, &value))
    app_info->set_package(value);
  if (app_dict->GetString(keys::kTizenApplicationRequiredVersionKey, &value)) {
    if (!value.empty()) {
      // TODO(wy80.choi): should consider minimum API version for each profile.
      utils::VersionNumber req_version(value);
      if (req_version < kMinimumAPIVersion) {
        app_info->set_required_version(kMinimumAPIVersion.ToString());
      } else {
        app_info->set_required_version(value);
      }
    }
  }
  std::string launch_mode;
  app_dict->GetString(keys::kTizenApplicationLaunchModeKey, &launch_mode);
  app_info->set_launch_mode(launch_mode);

  *output = std::static_pointer_cast<parser::ManifestData>(app_info);
  return true;
}

bool TizenApplicationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const TizenApplicationInfo& app_info =
      static_cast<const TizenApplicationInfo&>(data);

  if (!parser::ValidateTizenApplicationId(app_info.id())) {
    *error = "The id property of application element "
             "does not match the format\n";
    return false;
  }

  if (!parser::ValidateTizenPackageId(app_info.package())) {
    *error = "The package property of application element "
             "does not match the format\n";
    return false;
  }

  if (app_info.id().find(app_info.package()) != 0) {
    *error = "The application element property id "
             "does not start with package.\n";
    return false;
  }
  if (app_info.required_version().empty()) {
    *error = "The required_version property of application "
             "element does not exist.\n";
    return false;
  }
  utils::VersionNumber supported_version = parser::GetCurrentPlatformVersion();
  if (!supported_version.IsValid()) {
    *error = "Cannot retrieve supported API version from platform";
    return false;
  }
  utils::VersionNumber required_version(app_info.required_version());
  if (!required_version.IsValid()) {
    *error = "Cannot retrieve supported API version from widget";
    return false;
  }
  if (supported_version < required_version) {
    *error = "The required_version of Tizen Web API "
             "is not supported.\n";
    return false;
  }
  if (required_version >= kLaunchModeRequiredVersion) {
    if (app_info.launch_mode().empty()) {
      // FIXME for now, this const_cast is used, but it is not the best way.
      TizenApplicationInfo &tmp = const_cast<TizenApplicationInfo &>(app_info);
      tmp.set_launch_mode("single");  // default parameter
    }
    else if (app_info.launch_mode() != "caller" &&
             app_info.launch_mode() != "group" &&
             app_info.launch_mode() != "single") {
      *error = "Wrong value of launch mode";
      return false;
    }
  }
  else if(!app_info.launch_mode().empty()) {
    *error = "launch_mode attribute cannot be used for api version lower"
             " than 2.4";
    return false;
  }
  return true;
}

std::string TizenApplicationHandler::Key() const {
  return keys::kTizenApplicationKey;
}

bool TizenApplicationHandler::AlwaysParseForKey() const {
  return true;
}

}  // namespace parse
}  // namespace wgt
