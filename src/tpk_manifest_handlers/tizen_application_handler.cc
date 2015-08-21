// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/tizen_application_handler.h"

#include <cassert>
#include <map>
#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/values.h"
#include "utils/iri_util.h"
#include "utils/logging.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

namespace keys = tpk::manifest_keys;

namespace {

std::string GetParsedValue(const char* key_main, const char* key,
                           const parser::DictionaryValue& control_dict) {
  std::string tmp;
  const parser::DictionaryValue *dict;

  if (control_dict.GetDictionary(key_main, &dict))
    dict->GetString(key, &tmp);

  return tmp;
}

void ParseTizenApplicationAndStore(
    const parser::DictionaryValue& control_dict,
    TizenApplicationInfo* app_info) {

  std::string xmlns =
    GetParsedValue(keys::kXMLS,
                   keys::kManifestTextKey,
                   control_dict);

  std::string api_version =
    GetParsedValue(keys::kAPI,
                   keys::kManifestTextKey,
                   control_dict);

  std::string package =
    GetParsedValue(keys::kPackage,
                   keys::kManifestTextKey,
                   control_dict);

  std::string version =
    GetParsedValue(keys::kVersion,
                   keys::kManifestTextKey,
                   control_dict);

  std::string install_location =
    GetParsedValue(keys::kInstallLocation,
                   keys::kManifestTextKey,
                   control_dict);

  app_info->set_xmlns(xmlns);
  app_info->set_api_version(api_version);
  app_info->set_package(package);
  app_info->set_version(version);
  app_info->set_install_location(install_location);
}

}  // namespace

bool TizenApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<TizenApplicationInfo> app_info(new TizenApplicationInfo());
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kManifestKey, &value)) {
    *error = "Manifest element not found";
    return false;
  }

  if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);
    ParseTizenApplicationAndStore(*dict, app_info.get());
  } else {
    *error = "Cannot parse manifest element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(app_info);
  return true;
}

bool TizenApplicationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const TizenApplicationInfo& app_info =
       static_cast<const TizenApplicationInfo&>(data);

  if (app_info.xmlns().empty()) {
    *error = "The xmlns child element of manifest element is obligatory";
    return false;
  }

  const std::string& api_version = app_info.api_version();
  if (api_version.empty()) {
    *error =
        "The api-version child element of manifest element is obligatory";
    return false;
  }

  const std::string& package = app_info.package();
  if (package.empty()) {
    *error =
        "The package child element of manifest element is obligatory";
    return false;
  }

  const std::string& version = app_info.version();
  if (version.empty()) {
    *error =
        "The version child element of manifest element is obligatory";
    return false;
  }

  const std::string& install_location = app_info.install_location();
  if (install_location.empty()) {
    *error =
        "The install_location child element "
        "of manifest element is obligatory";
    return false;
  }
  return true;
}

std::string TizenApplicationHandler::Key() const {
  return keys::kManifestKey;
}

}   // namespace parse
}   // namespace tpk
