// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/package_handler.h"

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

namespace {
const char kNamespace[] = "@namespace";
const char kAPI[] = "@api-version";
const char kPackage[] = "@package";
const char kVersion[] = "@version";
const char kInstallLocation[] = "@install-location";
const char kAutoInstallLocation[] = "auto";
const char kManifestKey[] = "manifest";

void ParsePackageAndStore(
    const parser::DictionaryValue& manifest_dict,
    PackageInfo* pkg_info) {
  std::string xmlns;
  manifest_dict.GetString(kNamespace, &xmlns);
  std::string api_version;
  manifest_dict.GetString(kAPI, &api_version);
  std::string package;
  manifest_dict.GetString(kPackage, &package);
  std::string version;
  manifest_dict.GetString(kVersion, &version);
  std::string install_location;
  manifest_dict.GetString(kInstallLocation, &install_location);

  pkg_info->set_xmlns(xmlns);
  pkg_info->set_api_version(api_version);
  pkg_info->set_package(package);
  pkg_info->set_version(version);
  if (install_location.empty()) {
    pkg_info->set_install_location(kAutoInstallLocation);
  } else {
    pkg_info->set_install_location(install_location);
  }
}

}  // namespace


std::string PackageInfo::key() {
  return kManifestKey;
}

bool PackageHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<PackageInfo> pkg_info(new PackageInfo());
  parser::Value* value = nullptr;
  if (!manifest.Get(kManifestKey, &value)) {
    *error = "Manifest element not found";
    return false;
  }

  if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    ParsePackageAndStore(*dict, pkg_info.get());
  } else {
    *error = "Cannot parse manifest element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(pkg_info);
  return true;
}

bool PackageHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const PackageInfo& app_info =
       static_cast<const PackageInfo&>(data);

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

  return true;
}

std::string PackageHandler::Key() const {
  return kManifestKey;
}

}   // namespace parse
}   // namespace tpk
