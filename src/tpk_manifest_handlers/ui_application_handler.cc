// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/ui_application_handler.h"

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
const char kUIApplicationKey[] = "manifest.ui-application";

}  // namespace

namespace tpk {
namespace parse {

namespace ba = boost::algorithm;
namespace keys = tpk::application_keys;

namespace {
// app-control
const char kAppControlKey[] = "app-control";
const char kAppControlOperationKey[] = "operation";
const char kAppControlURIKey[] = "uri";
const char kAppControlMimeKey[] = "mime";
const char kAppControlNameChildKey[] = "@name";

// datacontrol
const char kDataControlKey[] = "datacontrol";
const char kDataControlAccessKey[] = "@access";
const char kDataControlProviderIDKey[] = "@providerid";
const char kDataControlTypeKey[] = "@type";

// icon
const char kIconKey[] = "icon";
const char kIconTextKey[] = "#text";

// image
const char kImageKey[] = "image";
const char kImageNameKey[] = "@name";
const char kImageLangKey[] = "@lang";

// label
const char kLabelKey[] = "label";
const char kLabelTextKey[] = "#text";
const char kLabelLangKey[] = "@lang";

// metadata
const char kMetaDataKey[] = "metadata";
const char kMetaDataKeyKey[] = "@key";
const char kMetaDataValueKey[] = "@value";

// ui-application
const char kUIApplicationAppIDKey[] = "@appid";
const char kUIApplicationExecKey[] = "@exec";
const char kUIApplicationLaunchModeKey[] = "@launch_mode";
const char kUIApplicationMultipleKey[] = "@multiple";
const char kUIApplicationNoDisplayKey[] = "@nodisplay";
const char kUIApplicationTaskManageKey[] = "@taskmanage";
const char kUIApplicationTypeKey[] = "@type";
const char kUIApplicationUIGadgetKey[] = "@ui-gadget";
const char kUIApplicationProcessPoolKey[] = "@process-pool";
const char kUIApplicationSubmodeKey[] = "@submode";
const char kUIApplicationSubmodeMainIDKey[] = "@submode-mainid";
const char kUIApplicationIndicatorDisplayKey[] = "@indicatordisplay";
const char kUIApplicationPortraitEffectImageKey[] = "@portrait-effectimage";
const char kUIApplicationLandscapeEffectImageKey[] = "@landscape-effectimage";
const char kUIApplicationEffectImageTypeKey[] = "@effectimage-type";
const char kUIApplicationHwAccelerationKey[] = "@hwacceleration";
const char kUIApplicationKey[] = "manifest.ui-application";

// manifest
const char kManifestKey[] = "manifest";

bool IsBooleanString(const std::string& value) {
  return ba::iequals(value, "true") || ba::iequals(value, "false");
}

}  // namespace

bool ParseAppControl(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
  std::string operation;
  const parser::DictionaryValue* operation_dict;
  if (dict->GetDictionary(kAppControlOperationKey,
                                 &operation_dict)) {
    operation_dict->GetString(
        kAppControlNameChildKey, &operation);
  }

  std::string uri;
  const parser::DictionaryValue* uri_dict;
  if (dict->GetDictionary(kAppControlURIKey,
                                 &uri_dict)) {
    uri_dict->GetString(
        kAppControlNameChildKey, &uri);
  }

  std::string mime;
  const parser::DictionaryValue* mime_dict;
  if (dict->GetDictionary(kAppControlMimeKey,
                                 &mime_dict)) {
    mime_dict->GetString(
        kAppControlNameChildKey, &mime);
  }

  info->app_control.emplace_back(operation, uri, mime);
  return true;
}

bool ParseDataControl(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
  std::string access;
  dict->GetString(kDataControlAccessKey, &access);
  std::string providerid;
  dict->GetString(kDataControlProviderIDKey, &providerid);
  std::string type;
  dict->GetString(kDataControlTypeKey, &type);
  info->data_control.emplace_back(access, providerid, type);
  return true;
}

bool ParseMetaData(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
  std::string key;
  dict->GetString(kMetaDataKeyKey, &key);
  std::string val;
  dict->GetString(kMetaDataValueKey, &val);
  info->meta_data.emplace_back(key, val);
  return true;
}

bool ParseAppIcon(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
  std::string icon_path;
  if (!dict->GetString(kIconTextKey, &icon_path))
    return false;
  info->app_icons.AddIcon(ApplicationIcon(icon_path));
  return true;
}

bool ParseAppImage(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
  std::string image_name;
  std::string image_lang;
  if (!dict->GetString(kImageNameKey, &image_name))
    return false;
  dict->GetString(kImageLangKey, &image_lang);
  info->app_images.images.emplace_back(image_name, image_lang);
  return true;
}

bool ParseLabel(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
  std::string text;
  dict->GetString(kLabelTextKey, &text);
  std::string xml_lang;
  dict->GetString(kLabelLangKey, &xml_lang);
  info->label.emplace_back(text, text, xml_lang);
  return true;
}

bool InitializeAppControlParsing(
    const parser::DictionaryValue& control_dict,
    UIApplicationSingleEntry* uiapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&control_dict, kAppControlKey, "")) {
    if (!ParseAppControl(item, uiapplicationinfo)) {
      *error = "Parsing AppControl failed";
      return false;
    }
  }
  return true;
}

bool InitializeDataControlParsing(
    const parser::DictionaryValue& control_dict,
    UIApplicationSingleEntry* uiapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&control_dict, kDataControlKey, "")) {
    if (!ParseDataControl(item, uiapplicationinfo)) {
      *error = "Parsing DataControl failed";
      return false;
    }
  }
  return true;
}

bool InitializeMetaDataParsing(
    const parser::DictionaryValue& app_dict,
    UIApplicationSingleEntry* uiapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kMetaDataKey, "")) {
    if (!ParseMetaData(item, uiapplicationinfo)) {
      *error = "Parsing Metadata failed";
      return false;
    }
  }
  return true;
}

bool InitializeIconParsing(
    const parser::DictionaryValue& app_dict,
    UIApplicationSingleEntry* uiapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kIconKey, "")) {
    if (!ParseAppIcon(item, uiapplicationinfo)) {
      *error = "Parsing Icon failed";
      return false;
    }
  }
  return true;
}

bool InitializeImageParsing(
    const parser::DictionaryValue& app_dict,
    UIApplicationSingleEntry* uiapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kImageKey, "")) {
    if (!ParseAppImage(item, uiapplicationinfo)) {
      *error = "Parsing Icon failed";
      return false;
    }
  }
  return true;
}

bool InitializeLabelParsing(
    const parser::DictionaryValue& control_dict,
    UIApplicationSingleEntry* uiapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&control_dict, kLabelKey, "")) {
    if (!ParseLabel(item, uiapplicationinfo)) {
      *error = "Parsing Label failed";
      return false;
    }
  }
  return true;
}

bool UIAppValidation(const UIApplicationSingleEntry& item,
                     const std::string& api_version, std::string* error) {
  if (item.ui_info.appid().empty()) {
    *error = "The appid child element of ui-application element is obligatory";
    return false;
  }

  const std::string& exec = item.ui_info.exec();
  if (exec.empty()) {
    *error = "The exec child element of ui-application element is obligatory";
    return false;
  }

  const std::string& launch_mode = item.ui_info.launch_mode();
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
    UIApplicationInfo &tmp = const_cast<UIApplicationInfo &>(item.ui_info);
    tmp.set_launch_mode("single");
  }

  const std::string& multiple = item.ui_info.multiple();
  if (multiple.empty()) {
    *error =
        "The multiple child element of ui-application element is obligatory";
    return false;
  }

  const std::string& nodisplay = item.ui_info.nodisplay();
  if (nodisplay.empty()) {
    *error =
        "The nodisplay child element of ui-application element is obligatory";
    return false;
  }

  const std::string& taskmanage = item.ui_info.taskmanage();
  if (taskmanage.empty()) {
    *error =
        "The taskmanage child element of "
        "ui-application element is obligatory";
    return false;
  }

  const std::string& type = item.ui_info.type();
  if (type.empty()) {
    *error =
        "The type child element of "
        "ui-application element is obligatory";
    return false;
  }
  return true;
}

bool AppControlValidation(
    const UIApplicationSingleEntry& it,
    std::string* error) {
  for (const auto& item : it.app_control) {
    if (item.operation().empty()) {
      *error =
          "The operation child element of app-control element is obligatory";
      return false;
    }
    if (!parser::utils::IsValidIRI(item.operation())) {
      *error =
          "The operation child element of app-control element is not valid url";
      return false;
    }
    const std::string& uri = item.uri();
    if (!uri.empty()) {
      if (!parser::utils::IsValidIRI(uri)) {
        *error =
            "The uri child element of app-control element is not valid url";
        return false;
      }
    }
  }
  return true;
}

bool DataControlValidation(
    const UIApplicationSingleEntry& it,
    std::string* error) {
  for (const auto& item : it.data_control) {
    if (item.access().empty()) {
      *error =
          "The access child element of datacontrol element is obligatory";
      return false;
    }

    const std::string& providerid = item.providerid();
    if (providerid.empty()) {
      *error =
          "The providerid child element of datacontrol element is obligatory";
      return false;
    }

    const std::string& type = item.type();
    if (type.empty()) {
      *error =
          "The type child element of datacontrol element is obligatory";
      return false;
    }
  }
  return true;
}

bool MetadataValidation(
    const UIApplicationSingleEntry& it,
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

bool LabelValidation(const UIApplicationSingleEntry& it, std::string* error) {
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

bool ParseUIApplicationAndStore(
    const parser::DictionaryValue& app_dict,
    UIApplicationSingleEntry* uiapplicationinfo,
    std::string* error) {
  std::string appid;
  if (app_dict.GetString(kUIApplicationAppIDKey, &appid))
    uiapplicationinfo->ui_info.set_appid(appid);
  std::string exec;
  if (app_dict.GetString(kUIApplicationExecKey, &exec))
    uiapplicationinfo->ui_info.set_exec(exec);
  std::string multiple;
  if (app_dict.GetString(kUIApplicationMultipleKey, &multiple))
    uiapplicationinfo->ui_info.set_multiple(multiple);
  std::string nodisplay;
  if (app_dict.GetString(kUIApplicationNoDisplayKey, &nodisplay))
    uiapplicationinfo->ui_info.set_nodisplay(nodisplay);
  std::string taskmanage;
  if (app_dict.GetString(kUIApplicationTaskManageKey, &taskmanage))
    uiapplicationinfo->ui_info.set_taskmanage(taskmanage);
  std::string type;
  if (app_dict.GetString(kUIApplicationTypeKey, &type))
    uiapplicationinfo->ui_info.set_type(type);
  std::string uigadget;
  if (app_dict.GetString(kUIApplicationUIGadgetKey, &uigadget))
    uiapplicationinfo->ui_info.set_uigadget(uigadget);
  std::string process_pool;
  if (app_dict.GetString(kUIApplicationProcessPoolKey, &process_pool)) {
    if (!IsBooleanString(process_pool)) {
      *error = "process_pool must be 'true' or 'false'";
      return false;
    }
    uiapplicationinfo->ui_info.set_process_pool(process_pool);
  }
  std::string submode;
  if (app_dict.GetString(kUIApplicationSubmodeKey, &submode)) {
    if (!IsBooleanString(submode)) {
      *error = "submode must be 'true' or 'false'";
      return false;
    }
    uiapplicationinfo->ui_info.set_submode(submode);
  }
  std::string submode_mainid;
  if (app_dict.GetString(kUIApplicationSubmodeMainIDKey, &submode_mainid))
    uiapplicationinfo->ui_info.set_submode_mainid(submode_mainid);
  std::string indicator_display;
  if (app_dict.GetString(kUIApplicationIndicatorDisplayKey,
                         &indicator_display)) {
    if (!IsBooleanString(indicator_display)) {
      *error = "indicatordisplay must be 'true' or 'false'";
      return false;
    }
    uiapplicationinfo->ui_info.set_indicator_display(indicator_display);
  }
  std::string portrait_effectimage;
  if (app_dict.GetString(kUIApplicationPortraitEffectImageKey,
                     &portrait_effectimage))
    uiapplicationinfo->ui_info.set_portrait_image(portrait_effectimage);
  std::string landscape_effectimage;
  if (app_dict.GetString(kUIApplicationLandscapeEffectImageKey,
                     &landscape_effectimage))
    uiapplicationinfo->ui_info.set_landscape_image(landscape_effectimage);
  std::string effectimage_type;
  if (app_dict.GetString(kUIApplicationEffectImageTypeKey, &effectimage_type))
    uiapplicationinfo->ui_info.set_effectimage_type(effectimage_type);

  std::string launch_mode;
  if (app_dict.GetString(kUIApplicationLaunchModeKey, &launch_mode)) {
    if (launch_mode.empty()) {
      *error = "launch_mode attribute is empty";
      return false;
    }
    uiapplicationinfo->ui_info.set_launch_mode(launch_mode);
  }

  std::string hwacceleration;
  if (app_dict.GetString(kUIApplicationHwAccelerationKey, &hwacceleration))
    uiapplicationinfo->ui_info.set_hwacceleration(hwacceleration);

  if (!InitializeAppControlParsing(app_dict, uiapplicationinfo, error) ||
     !InitializeDataControlParsing(app_dict, uiapplicationinfo, error) ||
     !InitializeMetaDataParsing(app_dict, uiapplicationinfo, error) ||
     !InitializeIconParsing(app_dict, uiapplicationinfo, error) ||
     !InitializeImageParsing(app_dict, uiapplicationinfo, error) ||
     !InitializeLabelParsing(app_dict, uiapplicationinfo, error)) {
    return false;
  }
  return true;
}

UIApplicationInfo::UIApplicationInfo()
    : multiple_("false"),
      nodisplay_("false"),
      taskmanage_("true"),
      indicator_display_("true"),
      effectimage_type_("image"),
      hwacceleration_("default") {
}

bool UIApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  if (!manifest.HasPath(kUIApplicationKey))
    return true;

  auto uiapplicationinfo = std::make_shared<UIApplicationInfoList>();

  for (const auto& ui_dict : parser::GetOneOrMany(
      manifest.value(), kUIApplicationKey, "")) {
    UIApplicationSingleEntry uiappentry;
    if (!ParseUIApplicationAndStore(*ui_dict, &uiappentry, error))
      return false;
    uiapplicationinfo->items.push_back(uiappentry);
  }

  *output = std::static_pointer_cast<parser::ManifestData>(uiapplicationinfo);
  return true;
}

bool UIApplicationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& handlers_output,
    std::string* error) const {
  const UIApplicationInfoList& elements =
      static_cast<const UIApplicationInfoList&>(data);


  std::shared_ptr<const PackageInfo> package_info =
      std::static_pointer_cast<const PackageInfo>(
          handlers_output.find(kManifestKey)->second);

  for (const auto& item : elements.items) {
    if (!UIAppValidation(item, package_info->api_version(), error) ||
       !AppControlValidation(item, error) ||
       !DataControlValidation(item, error) ||
       !MetadataValidation(item, error) ||
       !LabelValidation(item, error)) {
      return false;
    }
  }
  return true;
}

std::string UIApplicationInfo::key() {
  return kUIApplicationKey;
}

std::string UIApplicationHandler::Key() const {
  return kUIApplicationKey;
}

std::vector<std::string> UIApplicationHandler::PrerequisiteKeys() const {
  return { kManifestKey };
}

}   // namespace parse
}   // namespace tpk
