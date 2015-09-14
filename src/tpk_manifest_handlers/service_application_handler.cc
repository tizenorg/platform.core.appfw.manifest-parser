// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/service_application_handler.h"

#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/values.h"
#include "utils/iri_util.h"
#include "utils/logging.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

namespace keys = tpk::application_keys;

namespace {

std::string GetParsedValue(const char *key_main, const char *key,
                           const parser::DictionaryValue& control_dict) {
  std::string tmp;
  const parser::DictionaryValue *dict;

  if (control_dict.GetDictionary(key_main, &dict))
    dict->GetString(key, &tmp);

  return tmp;
}

bool ParseAppControl(
  const parser::DictionaryValue* dict,
  ServiceApplicationSingleEntry* info) {
  std::string operation;
  const parser::DictionaryValue* operation_dict;
  if (dict->GetDictionary(keys::kAppControlOperationKey,
                                 &operation_dict)) {
    operation_dict->GetString(
        keys::kAppControlNameChildKey, &operation);
  }

  std::string uri;
  const parser::DictionaryValue* uri_dict;
  if (dict->GetDictionary(keys::kAppControlURIKey,
                                 &uri_dict)) {
    uri_dict->GetString(
        keys::kAppControlNameChildKey, &uri);
  }

  std::string mime;
  const parser::DictionaryValue* mime_dict;
  if (dict->GetDictionary(keys::kAppControlMimeKey,
                                 &mime_dict)) {
    mime_dict->GetString(
        keys::kAppControlNameChildKey, &mime);
  }

  info->app_control.emplace_back(operation, uri, mime);
  return true;
}

bool ParseDataControl(
  const parser::DictionaryValue* dict,
  ServiceApplicationSingleEntry* info) {
  std::string access;
  dict->GetString(keys::kDataControlAccessKey, &access);
  std::string providerid;
  dict->GetString(keys::kDataControlProviderIDKey, &providerid);
  std::string type;
  dict->GetString(keys::kDataControlTypeKey, &type);
  info->data_control.emplace_back(access, providerid, type);
  return true;
}

bool ParseMetaData(
  const parser::DictionaryValue* dict,
  ServiceApplicationSingleEntry* info) {
  std::string key;
  dict->GetString(keys::kMetaDataKey, &key);
  std::string val;
  dict->GetString(keys::kMetaDataValueKey, &val);
  info->meta_data.emplace_back(key, val);
  return true;
}

bool ParseAppIcon(
  const parser::DictionaryValue* dict,
  ServiceApplicationSingleEntry* info) {
  std::string icon_path;
  if (!dict->GetString(keys::kIconTextKey, &icon_path))
    return false;
  info->app_icons.AddIcon(ApplicationIcon(icon_path));
  return true;
}

bool ParseLabel(
  const parser::DictionaryValue* dict,
  ServiceApplicationSingleEntry* info) {
  std::string text;
  dict->GetString(keys::kLabelKeyText, &text);
  std::string xml_lang;
  dict->GetString( keys::kLabelLangKey, &xml_lang);
  info->label.emplace_back(text, text, xml_lang);
  return true;
}

bool InitializeAppControlParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;

  if (app_dict.Get(keys::kAppControlKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseAppControl(dict, serviceapplicationinfo)) {
        *error = "Parsing AppControl failed";
          return false;
      }
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list) {
        if (item->GetAsDictionary(&dict)) {
          if (!ParseAppControl(dict, serviceapplicationinfo)) {
            *error = "Parsing AppControl failed";
            return false;
          }
        }
      }
    }
  }
  return true;
}

bool InitializeDataControlParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;

  if (app_dict.Get(keys::kDataControlKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseDataControl(dict, serviceapplicationinfo)) {
        *error = "Parsing DataControl failed";
        return false;
      }
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list) {
        if (item->GetAsDictionary(&dict)) {
          if (!ParseDataControl(dict, serviceapplicationinfo)) {
            *error = "Parsing DataControl failed";
            return false;
          }
        }
      }
    }
  }
  return true;
}

bool InitializeMetaDataParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;

  if (app_dict.Get(keys::kMetaData, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseMetaData(dict, serviceapplicationinfo)) {
        *error = "Parsing Metadata failed";
        return false;
      }
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list) {
        if (item->GetAsDictionary(&dict)) {
          if (!ParseMetaData(dict, serviceapplicationinfo)) {
            *error = "Parsing Metadata failed";
            return false;
          }
        }
      }
    }
  }
  return true;
}

bool InitializeIconParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;

  if (app_dict.Get(keys::kIconKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseAppIcon(dict, serviceapplicationinfo)) {
        *error = "Parsing Icon failed";
        return false;
      }
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list) {
        if (item->GetAsDictionary(&dict)) {
          if (!ParseAppIcon(dict, serviceapplicationinfo)) {
            *error = "Parsing Icon failed";
            return false;
          }
        }
      }
    }
  }
  return true;
}

bool InitializeLabelParsing(
    const parser::DictionaryValue& app_dict,
    ServiceApplicationSingleEntry* serviceapplicationinfo,
    std::string* error) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;

  if (app_dict.Get(keys::kLabelKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseLabel(dict, serviceapplicationinfo)) {
        *error = "Parsing Label failed";
        return false;
      }
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list) {
        if (item->GetAsDictionary(&dict)) {
          if (!ParseLabel(dict, serviceapplicationinfo)) {
            *error = "Parsing Label failed";
            return false;
          }
        }
      }
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

  const std::string& auto_restart = item.sa_info.auto_restart();
  if (auto_restart.empty()) {
    *error =
        "The auto_restart child element of "
        "service application element is obligatory";
    return false;
  }

  const std::string& exec = item.sa_info.exec();
  if (exec.empty()) {
    *error =
        "The exec child element of service application element is obligatory";
    return false;
  }

  const std::string& on_boot = item.sa_info.on_boot();
  if (on_boot.empty()) {
    *error =
        "The on_boot child element of "
        "service application element is obligatory";
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
  app_dict.GetString(keys::kServiceApplicationAppIDKey, &appid);
  std::string exec;
  app_dict.GetString(keys::kServiceApplicationExecKey, &exec);
  std::string auto_restart;
  app_dict.GetString(keys::kServiceApplicationAutoRestartKey, &auto_restart);
  std::string on_boot;
  app_dict.GetString(keys::kServiceApplicationOnBootKey, &on_boot);
  std::string type;
  app_dict.GetString(keys::kServiceApplicationTypeKey, &type);

  serviceapplicationinfo->sa_info.set_appid(appid);
  serviceapplicationinfo->sa_info.set_exec(exec);
  serviceapplicationinfo->sa_info.set_auto_restart(auto_restart);
  serviceapplicationinfo->sa_info.set_on_boot(on_boot);
  serviceapplicationinfo->sa_info.set_type(type);

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
  std::shared_ptr<ServiceApplicationInfoList>
                  serviceapplicationinfo(new ServiceApplicationInfoList());
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kServiceApplicationKey, &value))
    return true;

  if (value->GetType() == parser::Value::TYPE_LIST) {
    // multiple entries
    const parser::ListValue* list;
    value->GetAsList(&list);
    for (const auto& item : *list) {
      const parser::DictionaryValue* control_dict;
      if (!item->GetAsDictionary(&control_dict)) {
        *error = "Parsing service application element failed";
        return false;
      }

      ServiceApplicationSingleEntry serviceappentry;
      if (!ParseServiceApplicationAndStore(*control_dict,
                                          &serviceappentry,
                                          error))
        return false;
      serviceapplicationinfo->items.push_back(serviceappentry);
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    // single entry
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);

    ServiceApplicationSingleEntry serviceappentry;
    if (!ParseServiceApplicationAndStore(*dict,
                                        &serviceappentry,
                                        error))
      return false;
    serviceapplicationinfo->items.push_back(serviceappentry);
  } else {
    *error = "Cannot parse service application element";
    return false;
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

std::string ServiceApplicationHandler::Key() const {
  return keys::kServiceApplicationKey;
}

}   // namespace parse
}   // namespace tpk
