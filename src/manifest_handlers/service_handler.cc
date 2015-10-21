// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/service_handler.h"

#include <cassert>
#include <map>
#include <utility>
#include <set>

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_parser/manifest_util.h"
#include "manifest_parser/values.h"

namespace keys = wgt::application_widget_keys;

namespace {

bool ParseServiceContent(const parser::DictionaryValue* dict,
                         wgt::parse::ServiceInfo* service_info,
                         std::string* error) {
  const parser::Value* value = nullptr;
  if (!dict->Get(keys::kTizenServiceContentKey, &value)) {
    *error = "Missing tizen:content tag in tizen:service";
    return false;
  }
  std::string content;
  bool found = false;
  if (value->GetType() == parser::Value::TYPE_LIST) {
    const parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      if (!item->GetAsDictionary(&dict)) {
        *error = "Failed to parse tizen:content";
        return false;
      }
      if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
        continue;
      }
      if (found) {
        *error = "tizen:content element of tizen:service "
                 "should be declared only once";
        return false;
      }
      found = true;
      if (!dict->GetString(keys::kTizenServiceContentSrcKey, &content)) {
        *error =
            "Missing 'src' attribute in tizen:content tag in tizen:service";
        return false;
      }
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
      *error = "Missing tizen:content tag in tizen:service";
      return false;
    }
    if (!dict->GetString(keys::kTizenServiceContentSrcKey, &content)) {
      *error = "Missing 'src' attribute in tizen:content tag in tizen:service";
      return false;
    }
    found = true;
  } else {
    *error = "Failed to parse tizen:content element of service";
    return false;
  }
  if (!found) {
    *error = "Missing tizen:content tag in tizen:service";
    return false;
  }
  service_info->set_content(content);
  return true;
}

bool ParseServiceIcon(const parser::DictionaryValue* dict,
                      wgt::parse::ServiceInfo* service_info,
                      std::string* error) {
  const parser::Value* value = nullptr;
  if (!dict->Get(keys::kTizenServiceIconKey, &value))
    return true;
  std::string icon;
  if (value->GetType() == parser::Value::TYPE_LIST) {
    const parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    bool found = true;
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      if (!item->GetAsDictionary(&dict)) {
        *error = "Failed to parse tizen:icon";
        return false;
      }
      if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
        continue;
      }
      if (found) {
        *error = "tizen:icon element of tizen:service "
                 "should be declared only once";
        return false;
      }
      found = true;
      if (!dict->GetString(keys::kTizenServiceIconSrcKey, &icon)) {
        *error =
            "Missing 'src' attribute in tizen:icon tag in tizen:service";
        return false;
      }
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
      *error = "Missing tizen:icon tag in tizen:service";
      return false;
    }
    if (!dict->GetString(keys::kTizenServiceIconSrcKey, &icon)) {
      *error = "Missing 'src' attribute in tizen:icon tag in tizen:service";
      return false;
    }
  } else {
    *error = "Failed to parse tizen:icon element of service";
    return false;
  }
  service_info->set_icon(icon);
  return true;
}

bool ParseServiceDescription(const parser::DictionaryValue* dict,
                             wgt::parse::ServiceInfo* service_info,
                             std::string* error) {
  const parser::Value* value = nullptr;
  if (!dict->Get(keys::kTizenServiceDescriptionKey, &value))
    return true;
  std::string description;
  if (value->GetType() == parser::Value::TYPE_LIST) {
    const parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    bool found = true;
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      if (!item->GetAsDictionary(&dict)) {
        *error = "Failed to parse tizen:description";
        return false;
      }
      if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
        continue;
      }
      if (found) {
        *error = "tizen:description element of tizen:service "
                 "should be declared only once";
        return false;
      }
      found = true;
      dict->GetString(keys::kXmlTextKey, &description);
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    if (parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
      dict->GetString(keys::kXmlTextKey, &description);
    }
  } else {
    *error = "Failed to parse tizen:description element of service";
    return false;
  }
  service_info->set_description(description);
  return true;
}

bool ParseServiceCategory(const parser::DictionaryValue* dict,
                          wgt::parse::ServiceInfo* service_info,
                          std::string* error) {
  const parser::Value* value = nullptr;
  if (!dict->Get(keys::kTizenServiceCategoryKey, &value))
    return true;
  std::vector<std::string> categories;
  if (value->GetType() == parser::Value::TYPE_LIST) {
    const parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      if (!item->GetAsDictionary(&dict)) {
        *error = "Failed to parse tizen:category";
        return false;
      }
      if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
        continue;
      }
      std::string category;
      if (!dict->GetString(keys::kTizenServiceCategoryNameKey, &category)) {
        *error =
            "Missing 'name' attribute of tizen:category tag in tizen:service";
        return false;
      }
      categories.push_back(category);
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
      *error = "Missing tizen:category tag in tizen:service";
      return false;
    }
    std::string category;
    if (!dict->GetString(keys::kTizenServiceCategoryNameKey, &category)) {
      *error =
          "Missing 'name' attribute of tizen:category tag in tizen:service";
      return false;
    }
    categories.push_back(category);
  } else {
    *error = "Failed to parse tizen:category element of service";
    return false;
  }
  service_info->set_categories(categories);
  return true;
}

bool ParseServiceName(const parser::DictionaryValue* dict,
                      wgt::parse::ServiceInfo* service_info,
                      std::string* error) {
  const parser::Value* value = nullptr;
  if (!dict->Get(keys::kTizenServiceNameKey, &value)) {
    *error = "Missing tizen:name tag in tizen:service. "
             "At least one must be declared";
    return false;
  }
  wgt::parse::LangNameVector names;
  if (value->GetType() == parser::Value::TYPE_LIST) {
    const parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      if (!item->GetAsDictionary(&dict)) {
        *error = "Failed to parse tizen:name";
        return false;
      }
      if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
        continue;
      }
      std::string lang;
      std::string name;
      dict->GetString(keys::kXmlLangKey, &lang);
      dict->GetString(keys::kXmlTextKey, &name);
      names.emplace_back(lang, name);
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    if (parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
      std::string lang;
      std::string name;
      dict->GetString(keys::kXmlLangKey, &lang);
      dict->GetString(keys::kXmlTextKey, &name);
      names.emplace_back(lang, name);
    }
  } else {
    *error = "Failed to parse tizen:name element of service";
    return false;
  }
  if (names.empty()) {
    *error = "Cannot find tizen:name element for tizen:service";
    return false;
  }
  service_info->set_names(names);
  return true;
}

bool ParseServiceMetadata(const parser::DictionaryValue* dict,
                          wgt::parse::ServiceInfo* service_info,
                          std::string* error) {
  const parser::Value* value = nullptr;
  if (!dict->Get(keys::kTizenServiceMetadataKey, &value))
    return true;
  wgt::parse::KeyValueVector metadata_set;
  if (value->GetType() == parser::Value::TYPE_LIST) {
    const parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      if (!item->GetAsDictionary(&dict)) {
        *error = "Failed to parse tizen:metadata";
        return false;
      }
      if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
        continue;
      }
      std::string key;
      std::string value;
      dict->GetString(keys::kTizenServiceMetadataKeyKey, &key);
      dict->GetString(keys::kTizenServiceMetadataKeyValue, &value);
      metadata_set.emplace_back(key, value);
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    if (parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
      std::string key;
      std::string value;
      dict->GetString(keys::kTizenServiceMetadataKeyKey, &key);
      dict->GetString(keys::kTizenServiceMetadataKeyValue, &value);
      metadata_set.emplace_back(key, value);
    }
  } else {
    *error = "Failed to parse tizen:metadata element of service";
    return false;
  }
  service_info->set_metadata_set(metadata_set);
  return true;
}

std::unique_ptr<wgt::parse::ServiceInfo> ParseServiceInfo(
    const parser::DictionaryValue* dict, std::string* error) {
  std::string id;
  if (!dict->GetString(keys::kTizenServiceIdKey, &id)) {
    *error = "Cannot get appid for tizen:service";
    return nullptr;
  }

  std::unique_ptr<wgt::parse::ServiceInfo> service(
      new wgt::parse::ServiceInfo(id));

  std::string auto_restart = "false";
  if (dict->GetString(keys::kTizenServiceAutoRestartKey, &auto_restart))
    service->set_auto_restart(auto_restart == "true");

  std::string on_boot = "false";
  if (dict->GetString(keys::kTizenServiceOnBootKey, &on_boot))
    service->set_on_boot(on_boot == "true");

  if (!ParseServiceContent(dict, service.get(), error))
    return nullptr;

  if (!ParseServiceIcon(dict, service.get(), error))
    return nullptr;

  if (!ParseServiceDescription(dict, service.get(), error))
    return nullptr;

  if (!ParseServiceCategory(dict, service.get(), error))
    return nullptr;

  if (!ParseServiceName(dict, service.get(), error))
    return nullptr;

  if (!ParseServiceMetadata(dict, service.get(), error))
    return nullptr;

  return service;
}

}  // namespace

namespace wgt {

namespace parse {

ServiceInfo::ServiceInfo(const std::string& id, bool auto_restart, bool on_boot)
    : id_(id), auto_restart_(auto_restart), on_boot_(on_boot) {
}

ServiceInfo::~ServiceInfo() {}

ServiceHandler::ServiceHandler() {}

ServiceHandler::~ServiceHandler() {}

bool ServiceHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  const parser::Value* services = nullptr;
  if (!manifest.Get(keys::kTizenServiceKey, &services)) {
    return true;
  }

  std::shared_ptr<ServiceList> services_data(new ServiceList());

  if (services->IsType(parser::Value::TYPE_DICTIONARY)) {
    const parser::DictionaryValue* dict = nullptr;
    services->GetAsDictionary(&dict);
    if (parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix)) {
      auto service = ParseServiceInfo(dict, error);
      if (!service)
        return false;
      services_data->services.push_back(*service);
    }
  } else if (services->IsType(parser::Value::TYPE_LIST)) {
    const parser::ListValue* list;
    services->GetAsList(&list);
    for (parser::ListValue::const_iterator it = list->begin();
         it != list->end(); ++it) {
      const parser::DictionaryValue* dict = nullptr;
      if (!(**it).GetAsDictionary(&dict))
        continue;
      if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix))
        continue;
      auto service = ParseServiceInfo(dict, error);
      if (!service)
        return false;
      services_data->services.push_back(*service);
    }
  }

  *output = std::static_pointer_cast<parser::ManifestData>(services_data);
  return true;
}

bool ServiceHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const auto services = static_cast<const wgt::parse::ServiceList&>(data);

  for (auto& service : services.services) {
    if (!parser::ValidateTizenApplicationId(service.id())) {
      *error = "The id property of application element "
               "does not match the format\n";
      return false;
    }
  }

  return true;
}

std::string ServiceHandler::Key() const {
  return keys::kTizenServiceKey;
}

}  // namespace parse
}  // namespace wgt
