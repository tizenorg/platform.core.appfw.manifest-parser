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

void ParseServiceApplicationAndStore(
    const parser::DictionaryValue& control_dict,
    ServiceApplicationInfoList* serviceapplicationinfo) {

  std::string appid =
    GetParsedValue(keys::kServiceApplicationAppIDKey,
                   keys::kServiceApplicationKeyText,
                   control_dict);

  std::string auto_restart =
    GetParsedValue(keys::kServiceApplicationAutoRestartKey,
                   keys::kServiceApplicationKeyText,
                   control_dict);

  std::string exec =
    GetParsedValue(keys::kServiceApplicationExecKey,
                   keys::kServiceApplicationKeyText,
                   control_dict);

  std::string on_boot =
    GetParsedValue(keys::kServiceApplicationOnBootKey,
                   keys::kServiceApplicationKeyText,
                   control_dict);

  std::string type =
    GetParsedValue(keys::kServiceApplicationTypeKey,
                  keys::kServiceApplicationKeyText,
                  control_dict);

  serviceapplicationinfo->items.emplace_back(appid, auto_restart,
                                             exec, on_boot, type);
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
        *error = "Parsing label element failed";
        return false;
      }

      ParseServiceApplicationAndStore(*control_dict,
                                      serviceapplicationinfo.get());
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    // single entry
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);
    ParseServiceApplicationAndStore(*dict, serviceapplicationinfo.get());
  } else {
    *error = "Cannot parse label element";
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

  for (const auto& item : elements.items) {
    if (item.appid().empty()) {
      *error = "The appid child element of label element is obligatory";
      return false;
    }

    const std::string& auto_restart = item.auto_restart();
    if (auto_restart.empty()) {
      *error =
          "The auto_restart child element of label element is obligatory";
      return false;
    }

    const std::string& exec = item.exec();
    if (exec.empty()) {
      *error =
          "The exec child element of label element is obligatory";
      return false;
    }

    const std::string& on_boot = item.on_boot();
    if (on_boot.empty()) {
      *error =
          "The on_boot child element of label element is obligatory";
      return false;
    }

    const std::string& type = item.type();
    if (type.empty()) {
      *error =
          "The type child element of label element is obligatory";
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
