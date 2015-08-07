// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/ui_application_handler.h"

#include <cassert>
#include <map>
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

void ParseUIApplicationAndStore(
    const parser::DictionaryValue& control_dict,
    UIApplicationInfoList* uiapplicationinfo) {

  std::string appid = GetParsedValue(keys::kUIApplicationAppIDKey,
                                     keys::kUIApplicationKeyText,
                                     control_dict);

  std::string exec = GetParsedValue(keys::kUIApplicationExecKey,
                                    keys::kUIApplicationKeyText,
                                    control_dict);

  std::string multiple = GetParsedValue(keys::kUIApplicationMultipleKey,
                                        keys::kUIApplicationKeyText,
                                        control_dict);

  std::string nodisplay = GetParsedValue(keys::kUIApplicationNoDisplayKey,
                                         keys::kUIApplicationKeyText,
                                         control_dict);

  std::string taskmanage = GetParsedValue(keys::kUIApplicationTaskManageKey,
                                          keys::kUIApplicationKeyText,
                                          control_dict);

  std::string type = GetParsedValue(keys::kUIApplicationTypeKey,
                                    keys::kUIApplicationKeyText,
                                    control_dict);

  uiapplicationinfo->items.emplace_back(appid, exec, multiple,
                                        nodisplay, taskmanage, type);
}

}  // namespace

bool UIApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<UIApplicationInfoList>
                  uiapplicationinfo(new UIApplicationInfoList());
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kUIApplicationKey, &value))
    return true;

  if (value->GetType() == parser::Value::TYPE_LIST) {
    // multiple entries
    const parser::ListValue* list;
    value->GetAsList(&list);
    for (const auto& item : *list) {
      const parser::DictionaryValue* control_dict;
      if (!item->GetAsDictionary(&control_dict)) {
        *error = "Parsing ui-application element failed";
        return false;
      }

      ParseUIApplicationAndStore(*control_dict, uiapplicationinfo.get());
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    // single entry
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);

    ParseUIApplicationAndStore(*dict, uiapplicationinfo.get());
  } else {
    *error = "Cannot parse ui-application element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(uiapplicationinfo);
  return true;
}

bool UIApplicationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const UIApplicationInfoList& elements =
       static_cast<const UIApplicationInfoList&>(data);

  for (const auto& item : elements.items) {
    if (item.appid().empty()) {
      *error =
          "The appid child element "
          "of ui-application element is obligatory";
      return false;
    }

    const std::string& exec = item.exec();
    if (exec.empty()) {
      *error =
          "The exec child element of ui-application element is obligatory";
      return false;
    }

    const std::string& multiple = item.multiple();
    if (multiple.empty()) {
      *error = "The multiple child element of "
               "ui-application element is obligatory";
      return false;
    }

    const std::string& nodisplay = item.nodisplay();
    if (nodisplay.empty()) {
      *error =
          "The nodisplay child element of ui-application element is obligatory";
      return false;
    }

    const std::string& taskmanage = item.taskmanage();
    if (taskmanage.empty()) {
      *error =
          "The taskmanage child element of "
          "ui-application element is obligatory";
      return false;
    }

    const std::string& type = item.type();
    if (type.empty()) {
      *error =
          "The type child element of "
          "ui-application element is obligatory";
      return false;
    }
  }
  return true;
}

std::string UIApplicationHandler::Key() const {
  return keys::kUIApplicationKey;
}

}   // namespace parse
}   // namespace tpk
