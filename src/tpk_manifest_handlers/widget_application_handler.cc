// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/widget_application_handler.h"

#include <boost/algorithm/string/predicate.hpp>

#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/values.h"
#include "utils/iri_util.h"
#include "utils/logging.h"
#include "utils/version_number.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "tpk_manifest_handlers/package_handler.h"

namespace {

const utils::VersionNumber kLaunchModeRequiredVersion("2.4");
const char kWidgetApplicationKey[] = "manifest.widget-application";

}  // namespace

namespace tpk {
namespace parse {

namespace ba = boost::algorithm;
namespace keys = tpk::application_keys;

namespace {
// icon
const char kIconKey[] = "icon";
const char kIconTextKey[] = "#text";

// image
const char kImageKey[] = "image";
const char kImageNameKey[] = "@name";
const char kImageSectionKey[] = "@section";
const char kImageLangKey[] = "@lang";

// label
const char kLabelKey[] = "label";
const char kLabelTextKey[] = "#text";
const char kLabelLangKey[] = "@lang";

// metadata
const char kMetaDataKey[] = "metadata";
const char kMetaDataKeyKey[] = "@key";
const char kMetaDataValueKey[] = "@value";

// widget-application
const char kWidgetApplicationAppIDKey[] = "@appid";
const char kWidgetApplicationExecKey[] = "@exec";
const char kWidgetApplicationLaunchModeKey[] = "@launch_mode";
const char kWidgetApplicationMultipleKey[] = "@multiple";
const char kWidgetApplicationNoDisplayKey[] = "@nodisplay";
const char kWidgetApplicationTypeKey[] = "@type";
const char kWidgetApplicationHwAccelerationKey[] = "@hwacceleration";
const char kWidgetApplicationKey[] = "manifest.widget-application";

// manifest
const char kManifestKey[] = "manifest";

bool IsBooleanString(const std::string& value) {
  return ba::iequals(value, "true") || ba::iequals(value, "false");
}

}  // namespace

bool ParseMetaData(
  const parser::DictionaryValue* dict,
  WidgetApplicationSingleEntry* info) {
  std::string key;
  dict->GetString(kMetaDataKeyKey, &key);
  std::string val;
  dict->GetString(kMetaDataValueKey, &val);
  info->meta_data.emplace_back(key, val);
  return true;
}

bool ParseAppIcon(
  const parser::DictionaryValue* dict,
  WidgetApplicationSingleEntry* info) {
  std::string icon_path;
  if (!dict->GetString(kIconTextKey, &icon_path))
    return false;
  info->app_icons.AddIcon(ApplicationIcon(icon_path));
  return true;
}

bool ParseAppImage(
  const parser::DictionaryValue* dict,
  WidgetApplicationSingleEntry* info) {
  std::string image_name;
  std::string image_section;
  std::string image_lang;
  if (!dict->GetString(kImageNameKey, &image_name))
    return false;
  dict->GetString(kImageSectionKey, &image_section);
  dict->GetString(kImageLangKey, &image_lang);
  info->app_images.images.emplace_back(image_name, image_section, image_lang);
  return true;
}

bool ParseLabel(
  const parser::DictionaryValue* dict,
  WidgetApplicationSingleEntry* info) {
  std::string text;
  dict->GetString(kLabelTextKey, &text);
  std::string xml_lang;
  dict->GetString(kLabelLangKey, &xml_lang);
  info->label.emplace_back(text, text, xml_lang);
  return true;
}

bool InitializeMetaDataParsing(
    const parser::DictionaryValue& app_dict,
    WidgetApplicationSingleEntry* widgetapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kMetaDataKey, "")) {
    if (!ParseMetaData(item, widgetapplicationinfo)) {
      *error = "Parsing Metadata failed";
      return false;
    }
  }
  return true;
}

bool InitializeIconParsing(
    const parser::DictionaryValue& app_dict,
    WidgetApplicationSingleEntry* widgetapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kIconKey, "")) {
    if (!ParseAppIcon(item, widgetapplicationinfo)) {
      *error = "Parsing Icon failed";
      return false;
    }
  }
  return true;
}

bool InitializeImageParsing(
    const parser::DictionaryValue& app_dict,
    WidgetApplicationSingleEntry* widgetapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kImageKey, "")) {
    if (!ParseAppImage(item, widgetapplicationinfo)) {
      *error = "Parsing Image failed";
      return false;
    }
  }
  return true;
}

bool InitializeLabelParsing(
    const parser::DictionaryValue& control_dict,
    WidgetApplicationSingleEntry* widgetapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&control_dict, kLabelKey, "")) {
    if (!ParseLabel(item, widgetapplicationinfo)) {
      *error = "Parsing Label failed";
      return false;
    }
  }
  return true;
}

bool InitializeParsing(const parser::DictionaryValue& app_dict,
                       WidgetApplicationSingleEntry* widgetapplicationinfo,
                       std::string* error) {
  if (!InitializeMetaDataParsing(app_dict, widgetapplicationinfo, error))
    return false;
  if (!InitializeIconParsing(app_dict, widgetapplicationinfo, error))
    return false;
  if (!InitializeLabelParsing(app_dict, widgetapplicationinfo, error))
    return false;
  if (!InitializeImageParsing(app_dict, widgetapplicationinfo, error))
    return false;
  return true;
}

bool WidgetAppValidation(const WidgetApplicationSingleEntry& item,
                     const std::string& api_version, std::string* error) {
  if (item.widget_info.appid().empty()) {
    *error = "The appid child element of ui-application element is obligatory";
    return false;
  }

  const std::string& exec = item.widget_info.exec();
  if (exec.empty()) {
    *error = "The exec child element of ui-application element is obligatory";
    return false;
  }

  const std::string& launch_mode = item.widget_info.launch_mode();
  if (!launch_mode.empty()) {
    if (utils::VersionNumber(api_version) < kLaunchModeRequiredVersion) {
      *error = "launch_mode attribute cannot be used for api version lower"
              " than 2.4";
      return false;
    }
    if (launch_mode != "group" &&
        launch_mode != "caller" &&
        launch_mode != "single") {
      *error = "The improper value was given for ui-application launch_mode";
      return false;
    }
  } else {
    // FIXME currently const_cast used, but it is not the best way.
    WidgetApplicationInfo &tmp = const_cast<WidgetApplicationInfo &>(item.widget_info);
    tmp.set_launch_mode("single");
  }

  const std::string& multiple = item.widget_info.multiple();
  if (multiple.empty()) {
    *error =
        "The multiple child element of ui-application element is obligatory";
    return false;
  }

  const std::string& nodisplay = item.widget_info.nodisplay();
  if (nodisplay.empty()) {
    *error =
        "The nodisplay child element of ui-application element is obligatory";
    return false;
  }

  const std::string& type = item.widget_info.type();
  if (type.empty()) {
    *error =
        "The type child element of "
        "ui-application element is obligatory";
    return false;
  }
  return true;
}

bool MetadataValidation(
    const WidgetApplicationSingleEntry& it,
    std::string* error) {
  for (const auto& item : it.meta_data) {
    if (item.key().empty()) {
      *error =
          "The key child element of metadata element is obligatory";
      return false;
    }
  }
  return true;
}

bool LabelValidation(const WidgetApplicationSingleEntry& it, std::string* error) {
  for (const auto& item : it.label) {
    if (item.text().empty()) {
      *error = "The text child element of label element is obligatory";
      return false;
    }

    const std::string& name = item.name();
    if (name.empty()) {
      *error =
          "The name child element of label element is obligatory";
      return false;
    }
  }
  return true;
}

bool ParseWidgetApplicationAndStore(
    const parser::DictionaryValue& app_dict,
    WidgetApplicationSingleEntry* widgetapplicationinfo,
    std::string* error) {
  std::string appid;
  if (app_dict.GetString(kWidgetApplicationAppIDKey, &appid))
    widgetapplicationinfo->widget_info.set_appid(appid);
  std::string exec;
  if (app_dict.GetString(kWidgetApplicationExecKey, &exec))
    widgetapplicationinfo->widget_info.set_exec(exec);
  std::string multiple;
  if (app_dict.GetString(kWidgetApplicationMultipleKey, &multiple))
    widgetapplicationinfo->widget_info.set_multiple(multiple);
  std::string nodisplay;
  if (app_dict.GetString(kWidgetApplicationNoDisplayKey, &nodisplay))
    widgetapplicationinfo->widget_info.set_nodisplay(nodisplay);
  std::string type;
  if (app_dict.GetString(kWidgetApplicationTypeKey, &type))
    widgetapplicationinfo->widget_info.set_type(type);

  std::string launch_mode;
  if (app_dict.GetString(kWidgetApplicationLaunchModeKey, &launch_mode)) {
    if (launch_mode.empty()) {
      *error = "launch_mode attribute is empty";
      return false;
    }
    widgetapplicationinfo->widget_info.set_launch_mode(launch_mode);
  }

  std::string hwacceleration;
  if (app_dict.GetString(kWidgetApplicationHwAccelerationKey, &hwacceleration))
    widgetapplicationinfo->widget_info.set_hwacceleration(hwacceleration);

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

  *output = std::static_pointer_cast<parser::ManifestData>(widgetapplicationinfo);
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

std::string WidgetApplicationInfo::key() {
  return kWidgetApplicationKey;
}

std::string WidgetApplicationHandler::Key() const {
  return kWidgetApplicationKey;
}

std::vector<std::string> WidgetApplicationHandler::PrerequisiteKeys() const {
  return { kManifestKey };
}

}   // namespace parse
}   // namespace tpk
