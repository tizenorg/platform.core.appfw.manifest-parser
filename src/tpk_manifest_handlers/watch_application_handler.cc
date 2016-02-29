// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/watch_application_handler.h"

#include <boost/algorithm/string/predicate.hpp>

#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/utils/iri_util.h"
#include "manifest_parser/utils/logging.h"
#include "manifest_parser/utils/version_number.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "tpk_manifest_handlers/package_handler.h"

namespace tpk {
namespace parse {

namespace ba = boost::algorithm;
namespace keys = tpk::application_keys;

namespace {
// image
const char kImageKey[] = "image";
const char kImageNameKey[] = "@name";
const char kImageSectionKey[] = "@section";
const char kImageLangKey[] = "@lang";

// watch-application
const char kWatchApplicationAppIDKey[] = "@appid";
const char kWatchApplicationExecKey[] = "@exec";
const char kWatchApplicationLaunchModeKey[] = "@launch_mode";
const char kWatchApplicationMultipleKey[] = "@multiple";
const char kWatchApplicationNoDisplayKey[] = "@nodisplay";
const char kWatchApplicationHwAccelerationKey[] = "@hwacceleration";
const char kWatchApplicationKey[] = "manifest.watch-application";

// manifest
const char kManifestKey[] = "manifest";

}  // namespace

template<>
bool ParseMetaData(const parser::DictionaryValue& dict,
                   WatchApplicationSingleEntry* info, std::string*) {
  std::string key;
  dict.GetString(tpk_app_keys::kMetaDataKeyKey, &key);
  std::string val;
  dict.GetString(tpk_app_keys::kMetaDataValueKey, &val);
  info->meta_data.emplace_back(key, val);

  return true;
}

bool InitializeParsing(const parser::DictionaryValue& app_dict,
                       WatchApplicationSingleEntry* watchapplicationinfo,
                       std::string* error) {
  ParsingFuncPtr<WatchApplicationSingleEntry> parsingFunc =
      ParseMetaData<WatchApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kMetaDataKey,
      parsingFunc, watchapplicationinfo, error))
    return false;
  parsingFunc = ParseAppIcon<WatchApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kIconKey,
      parsingFunc, watchapplicationinfo, error))
    return false;
  parsingFunc = ParseLabel<WatchApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kLabelKey,
      parsingFunc, watchapplicationinfo, error))
    return false;
  parsingFunc = ParseAppImage<WatchApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kImageKey,
      parsingFunc, watchapplicationinfo, error))
    return false;
  parsingFunc = ParseCategory<WatchApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kCategoryKey,
      parsingFunc, watchapplicationinfo, error))
    return false;

  return true;
}

bool WatchAppValidation(const WatchApplicationSingleEntry& item,
                     const std::string&, std::string* error) {
  if (item.app_info.appid().empty()) {
    *error = "The appid child element of watch-application element "
        "is obligatory";
    return false;
  }

  const std::string& exec = item.app_info.exec();
  if (exec.empty()) {
    *error = "The exec child element of watch-application element "
        "is obligatory";
    return false;
  }

  const std::string& launch_mode = item.app_info.launch_mode();
  if (!launch_mode.empty()) {
    if (launch_mode != "group" &&
        launch_mode != "caller" &&
        launch_mode != "single") {
      *error = "The improper value was given for watch-application "
          "launch_mode";
      return false;
    }
  } else {
    // FIXME currently const_cast used, but it is not the best way.
    WatchApplicationInfo &tmp = const_cast<WatchApplicationInfo &>(
        item.app_info);
    tmp.set_launch_mode("single");
  }

  const std::string& multiple = item.app_info.multiple();
  if (multiple.empty()) {
    *error = "The multiple child element of watch-application element "
        "is obligatory";
    return false;
  }

  const std::string& nodisplay = item.app_info.nodisplay();
  if (nodisplay.empty()) {
    *error = "The nodisplay child element of watch-application element "
        "is obligatory";
    return false;
  }
  return true;
}

bool ParseWatchApplicationAndStore(
    const parser::DictionaryValue& app_dict,
    WatchApplicationSingleEntry* watchapplicationinfo,
    std::string* error) {
  std::string appid;
  if (app_dict.GetString(kWatchApplicationAppIDKey, &appid))
    watchapplicationinfo->app_info.set_appid(appid);
  std::string exec;
  if (app_dict.GetString(kWatchApplicationExecKey, &exec))
    watchapplicationinfo->app_info.set_exec(exec);
  std::string multiple;
  if (app_dict.GetString(kWatchApplicationMultipleKey, &multiple))
    watchapplicationinfo->app_info.set_multiple(multiple);
  std::string nodisplay;
  if (app_dict.GetString(kWatchApplicationNoDisplayKey, &nodisplay))
    watchapplicationinfo->app_info.set_nodisplay(nodisplay);

  std::string launch_mode;
  if (app_dict.GetString(kWatchApplicationLaunchModeKey, &launch_mode)) {
    if (launch_mode.empty()) {
      *error = "launch_mode attribute is empty";
      return false;
    }
    watchapplicationinfo->app_info.set_launch_mode(launch_mode);
  }

  std::string hwacceleration;
  if (app_dict.GetString(kWatchApplicationHwAccelerationKey, &hwacceleration))
    watchapplicationinfo->app_info.set_hwacceleration(hwacceleration);

  return InitializeParsing(app_dict, watchapplicationinfo, error);
}

WatchApplicationInfo::WatchApplicationInfo()
    : multiple_("false"),
      nodisplay_("false"),
      hwacceleration_("default") {
}

bool WatchApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  if (!manifest.HasPath(kWatchApplicationKey))
    return true;

  auto watchapplicationinfo = std::make_shared<WatchApplicationInfoList>();

  for (const auto& ui_dict : parser::GetOneOrMany(
      manifest.value(), kWatchApplicationKey, "")) {
    WatchApplicationSingleEntry watchappentry;
    if (!ParseWatchApplicationAndStore(*ui_dict, &watchappentry, error))
      return false;
    watchapplicationinfo->items.push_back(watchappentry);
  }

  *output = std::static_pointer_cast<parser::ManifestData>(
      watchapplicationinfo);
  return true;
}

bool WatchApplicationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& handlers_output,
    std::string* error) const {
  const WatchApplicationInfoList& elements =
      static_cast<const WatchApplicationInfoList&>(data);


  std::shared_ptr<const PackageInfo> package_info =
      std::static_pointer_cast<const PackageInfo>(
          handlers_output.find(kManifestKey)->second);

  for (const auto& item : elements.items) {
    if (!WatchAppValidation(item, package_info->api_version(), error) ||
       !MetadataValidation(item, error) ||
       !LabelValidation(item, error)) {
      return false;
    }
  }
  return true;
}

std::string WatchApplicationHandler::Key() const {
  return kWatchApplicationKey;
}

std::vector<std::string> WatchApplicationHandler::PrerequisiteKeys() const {
  return { kManifestKey };
}

}   // namespace parse
}   // namespace tpk
