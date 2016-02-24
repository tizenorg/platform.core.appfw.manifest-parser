// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/package_handler.h"

#include <cassert>
#include <map>
#include <set>
#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/utils/iri_util.h"
#include "manifest_parser/utils/logging.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

namespace {
const char kNamespace[] = "@namespace";
const char kAPI[] = "@api-version";
const char kPackage[] = "@package";
const char kVersion[] = "@version";
const char kType[] = "@type";
const char kInstallLocation[] = "@install-location";
const char kNodisplaySetting[] = "@nodisplay-setting";
const char kAutoInstallLocation[] = "auto";
const char kManifestKey[] = "manifest";
const char kLabelKey[] = "label";
const char kLabelLangKey[] = "@lang";
const char kLabelTextKey[] = "#text";
const char kPreload[] = "@preload";
const std::set<std::string> kInstallLocationAllowedValues =
    {"auto", "internal-only", "prefer-external"};
const std::set<std::string> kPackageTypeAllowedValues =
    {"tpk", "wgt", "rpm"};

bool ParsePackageAndStore(
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
  std::string type;
  manifest_dict.GetString(kType, &type);
  std::string install_location;
  manifest_dict.GetString(kInstallLocation, &install_location);
  std::string nodisplay_setting("false");
  manifest_dict.GetString(kNodisplaySetting, &nodisplay_setting);
  std::string preload;
  manifest_dict.GetString(kPreload, &preload);

  pkg_info->set_xmlns(xmlns);
  pkg_info->set_api_version(api_version);
  pkg_info->set_package(package);
  pkg_info->set_version(version);
  pkg_info->set_type(type);
  pkg_info->set_nodisplay_setting(nodisplay_setting);
  pkg_info->set_preload(preload);

  if (install_location.empty()) {
    pkg_info->set_install_location(kAutoInstallLocation);
  } else {
    if (kInstallLocationAllowedValues.find(install_location) ==
        kInstallLocationAllowedValues.end())
      return false;
    pkg_info->set_install_location(install_location);
  }

  for (auto label_dict : parser::GetOneOrMany(&manifest_dict, kLabelKey, "")) {
    std::string lang;
    std::string text;
    label_dict->GetString(kLabelLangKey, &lang);
    label_dict->GetString(kLabelTextKey, &text);
    pkg_info->AddLabel(lang, text);
  }
  return true;
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
    if (!ParsePackageAndStore(*dict, pkg_info.get())) {
      *error = "Cannot parse manifest element";
      return false;
    }
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
  if (!parser::ValidateTizenNativeId(package)) {
    *error = "The package name should be composed of alphanumeric characters "
             "optionally separate with dots.";
    return false;
  }

  if (!app_info.type().empty() &&
      kPackageTypeAllowedValues.find(app_info.type()) ==
      kPackageTypeAllowedValues.end()) {
    *error = "Invalid package type";
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
