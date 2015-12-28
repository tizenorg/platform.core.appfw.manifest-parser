// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/service_application_handler.h"

#include <boost/algorithm/string/predicate.hpp>

#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/values.h"
#include "utils/iri_util.h"
#include "utils/logging.h"
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
const char kServiceApplicationKeyText[] = "#text";

// app-control
const char kAppControlKey[] = "app-control";
const char kAppControlOperationKey[] = "operation";
const char kAppControlURIKey[] = "uri";
const char kAppControlMimeKey[] = "mime";
const char kAppControlNameChildKey[] = "@name";

// background-category
const char kBackgroundCategoryKey[] = "background-category";
const char kBackgroundCategoryValueKey[] = "@value";

// datacontrol
const char kDataControlKey[] = "datacontrol";
const char kDataControlAccessKey[] = "@access";
const char kDataControlProviderIDKey[] = "@providerid";
const char kDataControlTypeKey[] = "@type";

// icon
const char kIconKey[] = "icon";
const char kIconTextKey[] = "#text";

// label
const char kLabelKey[] = "label";
const char kLabelTextKey[] = "#text";
const char kLabelLangKey[] = "@lang";

// metadata
const char kMetaDataKey[] = "metadata";
const char kMetaDataKeyKey[] = "@key";
const char kMetaDataValueKey[] = "@value";

bool ParseAppControl(
  const parser::DictionaryValue* dict,
  ServiceApplicationSingleEntry* info) {
  for (const auto& item_operation : parser::GetOneOrMany(dict,
      kAppControlOperationKey, "")) {
    std::string operation;
    std::string uri;
    std::string mime;
    item_operation->GetString(kAppControlNameChildKey, &operation);

    auto uri_items = parser::GetOneOrMany(dict, kAppControlURIKey, "");
    auto mime_items = parser::GetOneOrMany(dict, kAppControlMimeKey, "");

    if (uri_items.empty()) uri_items.push_back(new parser::DictionaryValue());
    if (mime_items.empty()) mime_items.push_back(new parser::DictionaryValue());

    for (const auto& item_uri : uri_items) {
      item_uri->GetString(kAppControlNameChildKey, &uri);

      for (const auto& item_mime : mime_items) {
        item_mime->GetString(kAppControlNameChildKey, &mime);
        info->app_control.emplace_back(operation, uri, mime);
      }
    }
  }

  return true;
}

bool ParseBackgroundCategoryElement(
    const parser::DictionaryValue* dict,
    ServiceApplicationSingleEntry* info) {
  std::string value;

  if (!dict->GetString(kBackgroundCategoryValueKey, &value))
    return false;

  info->background_category.emplace_back(std::move(value));

  return true;
}

bool ParseDataControl(
  const parser::DictionaryValue* dict,
  ServiceApplicationSingleEntry* info) {
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
  ServiceApplicationSingleEntry* info) {
  std::string key;
  dict->GetString(kMetaDataKeyKey, &key);
  std::string val;
  dict->GetString(kMetaDataValueKey, &val);
  info->meta_data.emplace_back(key, val);
  return true;
}

bool ParseAppIcon(
  const parser::DictionaryValue* dict,
  ServiceApplicationSingleEntry* info) {
  std::string icon_path;
  if (!dict->GetString(kIconTextKey, &icon_path))
    return false;
  info->app_icons.AddIcon(ApplicationIcon(icon_path));
  return true;
}

bool ParseLabel(
  const parser::DictionaryValue* dict,
  ServiceApplicationSingleEntry* info) {
  std::string text;
  dict->GetString(kLabelTextKey, &text);
  std::string xml_lang;
  dict->GetString(kLabelLangKey, &xml_lang);
  info->label.emplace_back(text, text, xml_lang);
  return true;
}

bool InitializeAppControlParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kAppControlKey, "")) {
    if (!ParseAppControl(item, serviceapplicationinfo)) {
      *error = "Parsing AppControl failed";
      return false;
    }
  }
  return true;
}

bool InitializeBackgroundCategoryParsing(
    const parser::DictionaryValue& control_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&control_dict,
      kBackgroundCategoryKey, "")) {
    if (!ParseBackgroundCategoryElement(item, serviceapplicationinfo)) {
      *error = "Parsing background-category element failed";
      return false;
    }
  }
  return true;
}

bool InitializeDataControlParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kDataControlKey, "")) {
    if (!ParseDataControl(item, serviceapplicationinfo)) {
      *error = "Parsing DataControl failed";
      return false;
    }
  }
  return true;
}

bool InitializeMetaDataParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kMetaDataKey, "")) {
    if (!ParseMetaData(item, serviceapplicationinfo)) {
      *error = "Parsing Metadata failed";
      return false;
    }
  }
  return true;
}

bool InitializeIconParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kIconKey, "")) {
    if (!ParseAppIcon(item, serviceapplicationinfo)) {
      *error = "Parsing Icon failed";
      return false;
    }
  }
  return true;
}

bool InitializeLabelParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kLabelKey, "")) {
    if (!ParseLabel(item, serviceapplicationinfo)) {
      *error = "Parsing Label failed";
      return false;
    }
  }
  return true;
}

bool ServiceAppValidation(
    const ServiceApplicationSingleEntry& item,
    std::string* error) {
  if (item.sa_info.appid().empty()) {
    *error = "The appid child element of "
             "service application element is obligatory";
    return false;
  }
  const std::string& exec = item.sa_info.exec();
  if (exec.empty()) {
    *error =
        "The exec child element of service application element is obligatory";
    return false;
  }
  const std::string& type = item.sa_info.type();
  if (type.empty()) {
    *error =
        "The type child element of service application element is obligatory";
    return false;
  }
  return true;
}

bool AppControlValidation(
    const ServiceApplicationSingleEntry& it,
    std::string* error) {
  for (const auto& item : it.app_control) {
    if (item.operation().empty()) {
      *error =
          "The operation child element of app-control element is obligatory";
      return false;
    } else if (!parser::utils::IsValidIRI(item.operation())) {
      *error =
          "The operation child element of app-control element is not valid url";
      return false;
    }
  }
  return true;
}

bool DataControlValidation(
    const ServiceApplicationSingleEntry& it,
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
    const ServiceApplicationSingleEntry& it,
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

bool LabelValidation(
    const ServiceApplicationSingleEntry& it,
    std::string* error) {
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

  serviceapplicationinfo->sa_info.set_appid(appid);
  serviceapplicationinfo->sa_info.set_exec(exec);
  serviceapplicationinfo->sa_info.set_auto_restart(auto_restart);
  serviceapplicationinfo->sa_info.set_on_boot(on_boot);
  serviceapplicationinfo->sa_info.set_type(type);

  std::string process_pool;
  if (app_dict.GetString(kServiceApplicationProcessPoolKey, &process_pool)) {
    if (!ba::iequals(process_pool, "true") &&
        !ba::iequals(process_pool, "false")) {
      *error = "process_pool must be 'true' or 'false'";
      return false;
    }
    serviceapplicationinfo->sa_info.set_process_pool(process_pool);
  }

  if (!InitializeAppControlParsing(app_dict,
                                  serviceapplicationinfo,
                                  error) ||
     !InitializeDataControlParsing(app_dict,
                                   serviceapplicationinfo,
                                   error) ||
     !InitializeMetaDataParsing(app_dict,
                                serviceapplicationinfo,
                                error) ||
     !InitializeIconParsing(app_dict,
                            serviceapplicationinfo,
                            error) ||
     !InitializeLabelParsing(app_dict,
                             serviceapplicationinfo,
                             error) ||
     !InitializeBackgroundCategoryParsing(app_dict,
                             serviceapplicationinfo,
                             error)) {
    return false;
  }
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
