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
#include "utils/logging.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

TizenApplicationInfo::TizenApplicationInfo() {
}

TizenApplicationInfo::~TizenApplicationInfo() {
}

TizenApplicationHandler::TizenApplicationHandler() {}

TizenApplicationHandler::~TizenApplicationHandler() {}

bool TizenApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  if (!VerifyElementNamespace(
        manifest, keys::kTizenApplicationKey, parser::kTizenNamespacePrefix))
    return true;
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
  if (app_dict->GetString(keys::kTizenApplicationPackageKey, &value)) {
    app_info->set_package(value);
  }
  if (app_dict->GetString(keys::kTizenApplicationRequiredVersionKey, &value))
    app_info->set_required_version(value);

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
    LOG(ERROR) << "app_info->id() = " << app_info.id();
    LOG(ERROR) << "app_info->package() = " << app_info.package();
    return false;
  }
  if (app_info.required_version().empty()) {
    *error = "The required_version property of application "
             "element does not exist.\n";
    return false;
  }

  const std::string supported_version = parser::GetCurrentPlatformVersion();
  // TODO(w.kosowicz) write better mechanism of version comparison
  if (supported_version.empty() ||
      supported_version.compare(app_info.required_version()) < 0) {
    *error = "The required_version of Tizen Web API "
             "is not supported.\n";
    return false;
  }

  return true;
}

std::string TizenApplicationHandler::Key() const {
  return keys::kTizenApplicationKey;
}

bool TizenApplicationHandler::AlwaysParseForType() const {
  return true;
}

}  // namespace parse
}  // namespace wgt
