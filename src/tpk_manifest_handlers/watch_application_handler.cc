// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/watch_application_handler.h"

#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/utils/logging.h"
#include "manifest_parser/utils/version_number.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "tpk_manifest_handlers/package_handler.h"

namespace tpk {
namespace parse {

namespace keys = tpk::application_keys;

namespace {

// watch-application
const char kWatchApplicationAppIDKey[] = "@appid";
const char kWatchApplicationExecKey[] = "@exec";
const char kWatchApplicationLabelKey[] = "label";
const char kWatchApplicationTypeKey[] = "@type";
const char kWatchApplicationIconKey[] = "icon";
const char kWatchApplicationAmbientSupport[] = "@ambient-support";
const char kWatchApplicationKey[] = "manifest.watch-application";

const char kTrue[] = "true";
const char kFalse[] = "false";

// manifest
const char kManifestKey[] = "manifest";

}  // namespace

bool WatchAppValidation(const WatchApplicationSingleEntry& watch_app_info,
                        std::string* error) {
  if (watch_app_info.app_info.appid().empty()) {
    *error = "The appid attribute of watch-application element is obligatory";
    return false;
  }
  if (!parser::ValidateTizenNativeId(watch_app_info.app_info.appid())) {
    *error = "The application id should be composed of alphanumerics "
             "optionally separate with dots.";
    return false;
  }

  if (watch_app_info.app_info.exec().empty()) {
    *error = "The exec attribute of watch-application element is obligatory";
    return false;
  }
  const auto& ambient_support = watch_app_info.app_info.ambient_support();
  if (ambient_support != kTrue && ambient_support != kFalse) {
    *error = "ambient-support attribute should have 'true' or 'false' value";
    return false;
  }

  return true;
}

bool InitializeParsing(const parser::DictionaryValue& app_dict,
                       WatchApplicationSingleEntry* widgetapplicationinfo,
                       std::string* error) {
  ParsingFuncPtr<WatchApplicationSingleEntry> parsingFunc =
      ParseLabel<WatchApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kLabelKey,
                                parsingFunc, widgetapplicationinfo, error))
    return false;
  parsingFunc = ParseAppIcon<WatchApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kIconKey,
                                parsingFunc, widgetapplicationinfo, error))
    return false;
  parsingFunc = ParseAppImage<WatchApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kImageKey,
                                parsingFunc, widgetapplicationinfo, error))
    return false;
  return true;
}

bool ParseWatchApplication(
    const parser::DictionaryValue& app_dict,
    WatchApplicationSingleEntry* watch_app_info,
    std::string* error) {

  std::string appid;
  if (app_dict.GetString(kWatchApplicationAppIDKey, &appid))
    watch_app_info->app_info.set_appid(appid);

  std::string exec;
  if (app_dict.GetString(kWatchApplicationExecKey, &exec))
    watch_app_info->app_info.set_exec(exec);

  std::string ambient_support;
  if (app_dict.GetString(kWatchApplicationAmbientSupport, &ambient_support))
    watch_app_info->app_info.set_ambient_support(ambient_support);

  std::string icon;
  if (app_dict.GetString(kWatchApplicationIconKey, &icon))
    watch_app_info->app_info.set_icon(icon);

  std::string label;
  if (app_dict.GetString(kWatchApplicationLabelKey, &label))
    watch_app_info->app_info.set_label(label);

  std::string type;
  if (app_dict.GetString(kWatchApplicationTypeKey, &type))
    watch_app_info->app_info.set_type(type);

  return InitializeParsing(app_dict, watch_app_info, error);
}

WatchApplicationInfo::WatchApplicationInfo()
    : type_(""), ambient_support_(kFalse), icon_(""), label_("") {
}

bool WatchApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  if (!manifest.HasPath(kWatchApplicationKey))
    return true;


  auto watchapplicationinfo = std::make_shared<WatchApplicationInfoList>();

  for (const auto& watch_dict : parser::GetOneOrMany(
      manifest.value(), kWatchApplicationKey, "")) {
    WatchApplicationSingleEntry watchappentry;
    if (!ParseWatchApplication(*watch_dict, &watchappentry, error))
      return false;
    watchapplicationinfo->items.push_back(watchappentry);
  }

  if (watchapplicationinfo->items.size() > 1) {
    *error = "Multiple watch-application tags not supported.";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(
      watchapplicationinfo);
  return true;
}

bool WatchApplicationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& handlers_output,
    std::string* error) const {
  const WatchApplicationInfoList& watch_app_info =
      static_cast<const WatchApplicationInfoList&>(data);

  std::shared_ptr<const PackageInfo> package_info =
      std::static_pointer_cast<const PackageInfo>(
          handlers_output.find(kManifestKey)->second);

  for (const auto& item : watch_app_info.items) {
    if (!WatchAppValidation(item,  error)) {
      return false;
    }
  }

  return true;
}

std::string WatchApplicationInfo::key() {
  return kWatchApplicationKey;
}

std::string WatchApplicationHandler::Key() const {
  return kWatchApplicationKey;
}

std::vector<std::string> WatchApplicationHandler::PrerequisiteKeys() const {
  return { kManifestKey };
}

}   // namespace parse
}   // namespace tpk
