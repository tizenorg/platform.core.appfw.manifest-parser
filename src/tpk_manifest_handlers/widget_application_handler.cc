// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/widget_application_handler.h"

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

// widget-application
const char kWidgetApplicationAppIDKey[] = "@appid";
const char kWidgetApplicationExecKey[] = "@exec";
const char kWidgetApplicationLaunchModeKey[] = "@launch_mode";
const char kWidgetApplicationMultipleKey[] = "@multiple";
const char kWidgetApplicationNoDisplayKey[] = "@nodisplay";
const char kWidgetApplicationHwAccelerationKey[] = "@hwacceleration";
const char kWidgetApplicationKey[] = "manifest.widget-application";

// manifest
const char kManifestKey[] = "manifest";

}  // namespace

template<>
bool ParseMetaData(const parser::DictionaryValue& dict,
                   WidgetApplicationSingleEntry* info, std::string*) {
  std::string key;
  dict.GetString(tpk_app_keys::kMetaDataKeyKey, &key);
  std::string val;
  dict.GetString(tpk_app_keys::kMetaDataValueKey, &val);
  info->meta_data.emplace_back(key, val);

  return true;
}

bool InitializeParsing(const parser::DictionaryValue& app_dict,
                       WidgetApplicationSingleEntry* widgetapplicationinfo,
                       std::string* error) {
  ParsingFuncPtr<WidgetApplicationSingleEntry> parsingFunc =
      ParseMetaData<WidgetApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kMetaDataKey,
      parsingFunc, widgetapplicationinfo, error))
    return false;
  parsingFunc = ParseAppIcon<WidgetApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kIconKey,
      parsingFunc, widgetapplicationinfo, error))
    return false;
  parsingFunc = ParseLabel<WidgetApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kLabelKey,
      parsingFunc, widgetapplicationinfo, error))
    return false;
  parsingFunc = ParseAppImage<WidgetApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kImageKey,
      parsingFunc, widgetapplicationinfo, error))
    return false;
  parsingFunc = ParseCategory<WidgetApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kCategoryKey,
      parsingFunc, widgetapplicationinfo, error))
    return false;

  return true;
}

bool WidgetAppValidation(const WidgetApplicationSingleEntry& item,
                     const std::string&, std::string* error) {
  if (item.app_info.appid().empty()) {
    *error = "The appid child element of widget-application element "
        "is obligatory";
    return false;
  }

  const std::string& exec = item.app_info.exec();
  if (exec.empty()) {
    *error = "The exec child element of widget-application element "
        "is obligatory";
    return false;
  }

  const std::string& launch_mode = item.app_info.launch_mode();
  if (!launch_mode.empty()) {
    if (launch_mode != "group" &&
        launch_mode != "caller" &&
        launch_mode != "single") {
      *error = "The improper value was given for widget-application "
          "launch_mode";
      return false;
    }
  } else {
    // FIXME currently const_cast used, but it is not the best way.
    WidgetApplicationInfo &tmp = const_cast<WidgetApplicationInfo &>(
        item.app_info);
    tmp.set_launch_mode("single");
  }

  const std::string& multiple = item.app_info.multiple();
  if (multiple.empty()) {
    *error = "The multiple child element of widget-application element "
        "is obligatory";
    return false;
  }

  const std::string& nodisplay = item.app_info.nodisplay();
  if (nodisplay.empty()) {
    *error = "The nodisplay child element of widget-application element "
        "is obligatory";
    return false;
  }
  return true;
}

bool ParseWidgetApplicationAndStore(
    const parser::DictionaryValue& app_dict,
    WidgetApplicationSingleEntry* widgetapplicationinfo,
    std::string* error) {
  std::string appid;
  if (app_dict.GetString(kWidgetApplicationAppIDKey, &appid))
    widgetapplicationinfo->app_info.set_appid(appid);
  std::string exec;
  if (app_dict.GetString(kWidgetApplicationExecKey, &exec))
    widgetapplicationinfo->app_info.set_exec(exec);
  std::string multiple;
  if (app_dict.GetString(kWidgetApplicationMultipleKey, &multiple))
    widgetapplicationinfo->app_info.set_multiple(multiple);
  std::string nodisplay;
  if (app_dict.GetString(kWidgetApplicationNoDisplayKey, &nodisplay))
    widgetapplicationinfo->app_info.set_nodisplay(nodisplay);

  std::string launch_mode;
  if (app_dict.GetString(kWidgetApplicationLaunchModeKey, &launch_mode)) {
    if (launch_mode.empty()) {
      *error = "launch_mode attribute is empty";
      return false;
    }
    widgetapplicationinfo->app_info.set_launch_mode(launch_mode);
  }

  std::string hwacceleration;
  if (app_dict.GetString(kWidgetApplicationHwAccelerationKey, &hwacceleration))
    widgetapplicationinfo->app_info.set_hwacceleration(hwacceleration);

  return InitializeParsing(app_dict, widgetapplicationinfo, error);
}

WidgetApplicationInfo::WidgetApplicationInfo()
    : multiple_("false"),
      nodisplay_("false"),
      hwacceleration_("default") {
}

bool WidgetApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  if (!manifest.HasPath(kWidgetApplicationKey))
    return true;

  auto widgetapplicationinfo = std::make_shared<WidgetApplicationInfoList>();

  for (const auto& ui_dict : parser::GetOneOrMany(
      manifest.value(), kWidgetApplicationKey, "")) {
    WidgetApplicationSingleEntry widgetappentry;
    if (!ParseWidgetApplicationAndStore(*ui_dict, &widgetappentry, error))
      return false;
    widgetapplicationinfo->items.push_back(widgetappentry);
  }

  *output = std::static_pointer_cast<parser::ManifestData>(
      widgetapplicationinfo);
  return true;
}

bool WidgetApplicationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& handlers_output,
    std::string* error) const {
  const WidgetApplicationInfoList& elements =
      static_cast<const WidgetApplicationInfoList&>(data);


  std::shared_ptr<const PackageInfo> package_info =
      std::static_pointer_cast<const PackageInfo>(
          handlers_output.find(kManifestKey)->second);

  for (const auto& item : elements.items) {
    if (!WidgetAppValidation(item, package_info->api_version(), error) ||
       !MetadataValidation(item, error) ||
       !LabelValidation(item, error)) {
      return false;
    }
  }
  return true;
}

std::string WidgetApplicationHandler::Key() const {
  return kWidgetApplicationKey;
}

std::vector<std::string> WidgetApplicationHandler::PrerequisiteKeys() const {
  return { kManifestKey };
}

}   // namespace parse
}   // namespace tpk
