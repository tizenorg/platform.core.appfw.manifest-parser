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
const char kWatchApplicationIconKey[] = "icon";
const char kWatchApplicationAmbientSupport[] = "@ambient-support";
const char kWatchApplicationKey[] = "manifest.watch-application";

// manifest
const char kManifestKey[] = "manifest";

}  // namespace

bool WatchAppValidation(const WatchApplicationInfo& watch_app_info,
                     const std::string& api_version, std::string* error) {
  if (watch_app_info.appid().empty()) {
    *error = "The appid element of watch-application element is obligatory";
    return false;
  }

  if (watch_app_info.exec().empty()) {
    *error = "The exec element of watch-application element is obligatory";
    return false;
  }
  return true;
}

bool ParseWatchApplication(
    const parser::DictionaryValue& app_dict,
    WatchApplicationInfo* watch_app_info,
    std::string* error) {

  std::string appid;
  if (app_dict.GetString(kWatchApplicationAppIDKey, &appid))
    watch_app_info->set_appid(appid);

  std::string exec;
  if (app_dict.GetString(kWatchApplicationExecKey, &exec))
    watch_app_info->set_exec(exec);

  std::string ambient_support;
  if (app_dict.GetString(kWatchApplicationAmbientSupport, &ambient_support))
    watch_app_info->set_ambient_support(ambient_support);

  std::string icon;
  if (app_dict.GetString(kWatchApplicationIconKey, &icon))
    watch_app_info->set_icon(icon);

  std::string label;
  if (app_dict.GetString(kWatchApplicationLabelKey, &label))
    watch_app_info->set_label(label);

  return true;
}

WatchApplicationInfo::WatchApplicationInfo()
    : ambient_support_(""), icon_(""), label_("") {
}

bool WatchApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  if (!manifest.HasPath(kWatchApplicationKey))
    return true;

  auto watch_app_info = std::make_shared<WatchApplicationInfo>();
  auto watch_app_list =
      parser::GetOneOrMany(manifest.value(), kWatchApplicationKey, "");

  if (!watch_app_list.empty()) {
    if (!ParseWatchApplication(*(watch_app_list.front()),
                               watch_app_info.get(),
                               error)) {
      return false;
    }
  } else {
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(watch_app_info);
  return true;
}

bool WatchApplicationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& handlers_output,
    std::string* error) const {
  const WatchApplicationInfo& watch_app_info =
      static_cast<const WatchApplicationInfo&>(data);

  std::shared_ptr<const PackageInfo> package_info =
      std::static_pointer_cast<const PackageInfo>(
          handlers_output.find(kManifestKey)->second);

  return WatchAppValidation(watch_app_info, package_info->api_version(), error);
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
