// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/ui_application_handler.h"

#include <boost/algorithm/string/predicate.hpp>

#include <utility>

#include "manifest_parser/utils/logging.h"
#include "manifest_parser/utils/version_number.h"
#include "manifest_parser/manifest_util.h"
#include "manifest_parser/values.h"
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
// image
const char kImageKey[] = "image";
const char kImageNameKey[] = "@name";
const char kImageSectionKey[] = "@section";
const char kImageLangKey[] = "@lang";

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

bool ParseAppImage(const parser::DictionaryValue& dict,
                   UIApplicationSingleEntry* info,
                   std::string* error) {
  std::string image_name;
  std::string image_section;
  std::string image_lang;

  if (!dict.GetString(kImageNameKey, &image_name)) {
    *error = "Parsing Image failed";
    return false;
  }

  dict.GetString(kImageSectionKey, &image_section);
  dict.GetString(kImageLangKey, &image_lang);
  info->app_images.images.emplace_back(image_name, image_section, image_lang);

  return true;
}

bool InitializeParsing(const parser::DictionaryValue& app_dict,
                       UIApplicationSingleEntry* uiapplicationinfo,
                       std::string* error) {
  ParsingFuncPtr<UIApplicationSingleEntry> parsingFunc =
      ParseAppControl<UIApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kAppControlKey,
      parsingFunc, uiapplicationinfo, error))
    return false;
  parsingFunc = ParseDataControl<UIApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kDataControlKey,
      parsingFunc, uiapplicationinfo, error))
    return false;
  parsingFunc = ParseMetaData<UIApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kMetaDataKey,
      parsingFunc, uiapplicationinfo, error))
    return false;
  parsingFunc = ParseAppIcon<UIApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kIconKey,
      parsingFunc, uiapplicationinfo, error))
    return false;
  parsingFunc = ParseLabel<UIApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kLabelKey,
      parsingFunc, uiapplicationinfo, error))
    return false;
  parsingFunc = ParseAppImage;
  if (!InitializeParsingElement(app_dict, kImageKey,
      parsingFunc, uiapplicationinfo, error))
    return false;
  parsingFunc = ParseBackgroundCategoryElement<UIApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kBackgroundCategoryKey,
      parsingFunc, uiapplicationinfo, error))
    return false;

  return true;
}

bool UIAppValidation(const UIApplicationSingleEntry& item,
                     const std::string& api_version, std::string* error) {
  if (item.app_info.appid().empty()) {
    *error = "The appid child element of ui-application element is obligatory";
    return false;
  }

  const std::string& exec = item.app_info.exec();
  if (exec.empty()) {
    *error = "The exec child element of ui-application element is obligatory";
    return false;
  }

  const std::string& launch_mode = item.app_info.launch_mode();
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
    UIApplicationInfo &tmp = const_cast<UIApplicationInfo &>(item.app_info);
    tmp.set_launch_mode("single");
  }

  const std::string& multiple = item.app_info.multiple();
  if (multiple.empty()) {
    *error =
        "The multiple child element of ui-application element is obligatory";
    return false;
  }

  const std::string& nodisplay = item.app_info.nodisplay();
  if (nodisplay.empty()) {
    *error =
        "The nodisplay child element of ui-application element is obligatory";
    return false;
  }

  const std::string& taskmanage = item.app_info.taskmanage();
  if (taskmanage.empty()) {
    *error =
        "The taskmanage child element of "
        "ui-application element is obligatory";
    return false;
  }

  const std::string& type = item.app_info.type();
  if (type.empty()) {
    *error =
        "The type child element of "
        "ui-application element is obligatory";
    return false;
  }
  return true;
}

bool ParseUIApplicationAndStore(
    const parser::DictionaryValue& app_dict,
    UIApplicationSingleEntry* uiapplicationinfo,
    std::string* error) {
  std::string appid;
  if (app_dict.GetString(kUIApplicationAppIDKey, &appid))
    uiapplicationinfo->app_info.set_appid(appid);
  std::string exec;
  if (app_dict.GetString(kUIApplicationExecKey, &exec))
    uiapplicationinfo->app_info.set_exec(exec);
  std::string multiple;
  if (app_dict.GetString(kUIApplicationMultipleKey, &multiple))
    uiapplicationinfo->app_info.set_multiple(multiple);
  std::string nodisplay;
  if (app_dict.GetString(kUIApplicationNoDisplayKey, &nodisplay))
    uiapplicationinfo->app_info.set_nodisplay(nodisplay);
  std::string taskmanage;
  if (app_dict.GetString(kUIApplicationTaskManageKey, &taskmanage))
    uiapplicationinfo->app_info.set_taskmanage(taskmanage);
  std::string type;
  if (app_dict.GetString(kUIApplicationTypeKey, &type))
    uiapplicationinfo->app_info.set_type(type);
  std::string uigadget;
  if (app_dict.GetString(kUIApplicationUIGadgetKey, &uigadget))
    uiapplicationinfo->app_info.set_uigadget(uigadget);
  std::string process_pool;
  if (app_dict.GetString(kUIApplicationProcessPoolKey, &process_pool)) {
    if (!IsBooleanString(process_pool)) {
      *error = "process_pool must be 'true' or 'false'";
      return false;
    }
    uiapplicationinfo->app_info.set_process_pool(process_pool);
  }
  std::string submode;
  if (app_dict.GetString(kUIApplicationSubmodeKey, &submode)) {
    if (!IsBooleanString(submode)) {
      *error = "submode must be 'true' or 'false'";
      return false;
    }
    uiapplicationinfo->app_info.set_submode(submode);
  }
  std::string submode_mainid;
  if (app_dict.GetString(kUIApplicationSubmodeMainIDKey, &submode_mainid))
    uiapplicationinfo->app_info.set_submode_mainid(submode_mainid);
  std::string indicator_display;
  if (app_dict.GetString(kUIApplicationIndicatorDisplayKey,
                         &indicator_display)) {
    if (!IsBooleanString(indicator_display)) {
      *error = "indicatordisplay must be 'true' or 'false'";
      return false;
    }
    uiapplicationinfo->app_info.set_indicator_display(indicator_display);
  }
  std::string portrait_effectimage;
  if (app_dict.GetString(kUIApplicationPortraitEffectImageKey,
                     &portrait_effectimage))
    uiapplicationinfo->app_info.set_portrait_image(portrait_effectimage);
  std::string landscape_effectimage;
  if (app_dict.GetString(kUIApplicationLandscapeEffectImageKey,
                     &landscape_effectimage))
    uiapplicationinfo->app_info.set_landscape_image(landscape_effectimage);
  std::string effectimage_type;
  if (app_dict.GetString(kUIApplicationEffectImageTypeKey, &effectimage_type))
    uiapplicationinfo->app_info.set_effectimage_type(effectimage_type);

  std::string launch_mode;
  if (app_dict.GetString(kUIApplicationLaunchModeKey, &launch_mode)) {
    if (launch_mode.empty()) {
      *error = "launch_mode attribute is empty";
      return false;
    }
    uiapplicationinfo->app_info.set_launch_mode(launch_mode);
  }

  std::string hwacceleration;
  if (app_dict.GetString(kUIApplicationHwAccelerationKey, &hwacceleration))
    uiapplicationinfo->app_info.set_hwacceleration(hwacceleration);

  return InitializeParsing(app_dict, uiapplicationinfo, error);
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
