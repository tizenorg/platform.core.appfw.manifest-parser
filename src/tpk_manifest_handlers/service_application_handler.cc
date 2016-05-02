// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/service_application_handler.h"

#include <boost/algorithm/string/predicate.hpp>

#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/utils/logger.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

namespace ba = boost::algorithm;
namespace keys = tpk::application_keys;

namespace {
const char kServiceApplicationKey[] = "manifest.service-application";

// service-application
const char kServiceApplicationAppIDKey[] = "@appid";
const char kServiceApplicationAutoRestartKey[] = "@auto-restart";
const char kServiceApplicationExecKey[] = "@exec";
const char kServiceApplicationOnBootKey[] = "@on-boot";
const char kServiceApplicationTypeKey[] = "@type";
const char kServiceApplicationProcessPoolKey[] = "@process-pool";
const char kServiceApplicationMultipleKey[] = "@multiple";
const char kServiceApplicationTaskManageKey[] = "@taskmanage";
const char kServiceApplicationKeyText[] = "#text";

const char kTrue[] = "true";
const char kFalse[] = "false";

bool ServiceAppValidation(
    const ServiceApplicationSingleEntry& item,
    std::string* error) {
  if (item.app_info.appid().empty()) {
    *error = "The appid child element of "
             "service application element is obligatory";
    return false;
  }
  if (!parser::ValidateTizenNativeId(item.app_info.appid())) {
    *error = "The application id should be composed of alphanumerics "
             "optionally separate with dots.";
    return false;
  }

  const std::string& exec = item.app_info.exec();
  if (exec.empty()) {
    *error =
        "The exec child element of service application element is obligatory";
    return false;
  }
  const std::string& type = item.app_info.type();
  if (type.empty()) {
    *error =
        "The type child element of service application element is obligatory";
    return false;
  }
  const std::string& multiple = item.app_info.multiple();
  if (multiple != kTrue && multiple != kFalse) {
    *error = "multiple attribute should have 'true' or 'false' value";
    return false;
  }
  const std::string& taskmanage = item.app_info.taskmanage();
  if (taskmanage != kTrue && taskmanage != kFalse) {
    *error = "taskmanage attribute should have 'true' or 'false' value";
    return false;
  }
  const std::string& on_boot = item.app_info.on_boot();
  if (on_boot != kTrue && on_boot != kFalse) {
    *error = "on-boot attribute should have 'true' or 'false' value";
    return false;
  }
  const std::string& auto_restart = item.app_info.auto_restart();
  if (auto_restart != kTrue && auto_restart != kFalse) {
    *error = "auto-restart attribute should have 'true' or 'false' value";
    return false;
  }

  return true;
}

bool ParseServiceApplicationAndStore(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  std::string appid;
  app_dict.GetString(kServiceApplicationAppIDKey, &appid);
  std::string exec;
  app_dict.GetString(kServiceApplicationExecKey, &exec);
  std::string auto_restart("false");
  app_dict.GetString(kServiceApplicationAutoRestartKey, &auto_restart);
  std::string on_boot("false");
  app_dict.GetString(kServiceApplicationOnBootKey, &on_boot);
  std::string type;
  app_dict.GetString(kServiceApplicationTypeKey, &type);
  std::string multiple("false");
  app_dict.GetString(kServiceApplicationMultipleKey, &multiple);
  std::string taskmanage("true");
  app_dict.GetString(kServiceApplicationTaskManageKey, &taskmanage);

  serviceapplicationinfo->app_info.set_appid(appid);
  serviceapplicationinfo->app_info.set_exec(exec);
  serviceapplicationinfo->app_info.set_auto_restart(auto_restart);
  serviceapplicationinfo->app_info.set_on_boot(on_boot);
  serviceapplicationinfo->app_info.set_type(type);
  serviceapplicationinfo->app_info.set_multiple(multiple);
  serviceapplicationinfo->app_info.set_taskmanage(taskmanage);

  std::string process_pool;
  if (app_dict.GetString(kServiceApplicationProcessPoolKey, &process_pool)) {
    if (!ba::iequals(process_pool, "true") &&
        !ba::iequals(process_pool, "false")) {
      *error = "process_pool must be 'true' or 'false'";
      return false;
    }
    serviceapplicationinfo->app_info.set_process_pool(process_pool);
  }

  ParsingFuncPtr<ServiceApplicationSingleEntry> parsingFunc =
      ParseAppControl<ServiceApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kAppControlKey,
      parsingFunc, serviceapplicationinfo, error))
    return false;
  parsingFunc = ParseDataControl<ServiceApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kDataControlKey,
      parsingFunc, serviceapplicationinfo, error))
    return false;
  parsingFunc = ParseMetaData<ServiceApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kMetaDataKey,
      parsingFunc, serviceapplicationinfo, error))
    return false;
  parsingFunc = ParseAppIcon<ServiceApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kIconKey,
      parsingFunc, serviceapplicationinfo, error))
    return false;
  parsingFunc = ParseLabel<ServiceApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kLabelKey,
      parsingFunc, serviceapplicationinfo, error))
    return false;
  parsingFunc = ParseBackgroundCategoryElement<ServiceApplicationSingleEntry>;
  if (!InitializeParsingElement(app_dict, tpk_app_keys::kBackgroundCategoryKey,
      parsingFunc, serviceapplicationinfo, error))
    return false;

  return true;
}

}  // namespace

bool ServiceApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  if (!manifest.HasPath(kServiceApplicationKey))
    return true;
  std::shared_ptr<ServiceApplicationInfoList>
                  serviceapplicationinfo(new ServiceApplicationInfoList());
  for (auto& item : parser::GetOneOrMany(manifest.value(),
                                         kServiceApplicationKey, "")) {
    ServiceApplicationSingleEntry serviceappentry;
    if (!ParseServiceApplicationAndStore(*item,
                                        &serviceappentry,
                                        error))
      return false;
    serviceapplicationinfo->items.push_back(serviceappentry);
  }
  *output = std::static_pointer_cast
            <parser::ManifestData>(serviceapplicationinfo);
  return true;
}

bool ServiceApplicationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const ServiceApplicationInfoList& elements =
       static_cast<const ServiceApplicationInfoList&>(data);

  for (auto& item : elements.items) {
    if (!ServiceAppValidation(item, error) ||
       !AppControlValidation(item, error) ||
       !DataControlValidation(item, error) ||
       !MetadataValidation(item, error) ||
       !LabelValidation(item, error)) {
      return false;
    }
  }
  return true;
}


std::string ServiceApplicationInfo::key() {
  return kServiceApplicationKey;
}

std::string ServiceApplicationHandler::Key() const {
  return kServiceApplicationKey;
}

}   // namespace parse
}   // namespace tpk
