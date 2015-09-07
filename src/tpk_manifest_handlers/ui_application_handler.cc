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

// Parsing AppControl
bool ParseAppControl(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
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

// Parsing DataControl
bool ParseDataControl(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
  std::string access;
  const parser::DictionaryValue* access_dict;
  if (dict->GetDictionary(keys::kDataControlAccessKey,
                                 &access_dict)) {
    access_dict->GetString(
        keys::kDataControlTypeChildNameAttrKey, &access);
  }

  std::string providerid;
  const parser::DictionaryValue* providerid_dict;
  if (dict->GetDictionary(keys::kDataControlProviderIDKey,
                                 &providerid_dict)) {
    providerid_dict->GetString(
        keys::kDataControlTypeChildNameAttrKey, &providerid);
  }

  std::string type;
  const parser::DictionaryValue* type_dict;
  if (dict->GetDictionary(keys::kDataControlTypeKey,
                                 &type_dict)) {
    type_dict->GetString(
        keys::kDataControlTypeChildNameAttrKey, &type);
  }

  info->data_control.emplace_back(access, providerid, type);
  return true;
}

// Parsing MetaData
bool ParseMetaData(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
  std::string key;
  const parser::DictionaryValue* key_dict;
  if (dict->GetDictionary(keys::kMetaDataKey,
                                 &key_dict)) {
    key_dict->GetString(
        keys::kMetaDataTypeChildNameAttrKey, &key);
  }

  std::string val;
  const parser::DictionaryValue* value_dict;
  if (dict->GetDictionary(keys::kMetaDataValueKey,
                                 &value_dict)) {
    value_dict->GetString(
        keys::kMetaDataTypeChildNameAttrKey, &val);
  }

  info->meta_data.emplace_back(key, val);
  return true;
}

// Parsing Icon
bool ExtractIconSrc(const parser::DictionaryValue* dict, std::string* value,
                    std::string* error) {
  std::string src;
  if (!dict->GetString(keys::kIconSrcKey, &src)) {
    *error = "Cannot find mandatory key. Key name: @src";
    return true;
  }
  *value = src;
  return  true;
}

bool ParseAppIcon(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info,
  std::string* error) {
  std::string icon_path;
  if (!ExtractIconSrc(dict, &icon_path, error)) {
        return true;
  }
  info->app_icons.AddIcon(ApplicationIcon(icon_path));
  return true;
}

// Parsing Label
bool ParseLabel(
  const parser::DictionaryValue* dict,
  UIApplicationSingleEntry* info) {
  std::string text;
  const parser::DictionaryValue* text_dict;
  if (dict->GetDictionary(keys::kLableKey,
                                 &text_dict)) {
    text_dict->GetString(
        keys::kLableKeyText, &text);
  }

  std::string name;
  const parser::DictionaryValue* name_dict;
  if (dict->GetDictionary(keys::kLableKey,
                                 &name_dict)) {
    name_dict->GetString(
        keys::kLableKeyName, &name);
  }

  std::string xml_lang;
  const parser::DictionaryValue* xml_lang_dict;
  if (dict->GetDictionary(keys::kLableLangKey,
                                 &xml_lang_dict)) {
    xml_lang_dict->GetString(
        keys::kLableKeyText, &xml_lang);
  }

  info->label.emplace_back(text, name, xml_lang);
  return true;
}

void ParseUIApplicationAndStore(
    const parser::DictionaryValue& control_dict,
    UIApplicationSingleEntry* uiapplicationinfo,
    std::string* error) {

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

  uiapplicationinfo->ui_info.set_appid(appid);
  uiapplicationinfo->ui_info.set_exec(exec);
  uiapplicationinfo->ui_info.set_multiple(multiple);
  uiapplicationinfo->ui_info.set_nodisplay(nodisplay);
  uiapplicationinfo->ui_info.set_taskmanage(taskmanage);
  uiapplicationinfo->ui_info.set_type(type);

  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  // AppControl
  if (control_dict.Get(keys::kAppControlKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseAppControl(dict, uiapplicationinfo))
          *error = "Parsing AppControl failed";
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          if (!ParseAppControl(dict, uiapplicationinfo))
            *error = "Parsing AppControl failed";
    }
  }
  // DataControl
  if (control_dict.Get(keys::kDataControlKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseDataControl(dict, uiapplicationinfo))
          *error = "Parsing DataControl failed";
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          if (!ParseDataControl(dict, uiapplicationinfo))
            *error = "Parsing DataControl failed";
    }
  }
  // MataData
  if (control_dict.Get(keys::kMetaData, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseMetaData(dict, uiapplicationinfo))
          *error = "Parsing MetaData failed";
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          if (!ParseMetaData(dict, uiapplicationinfo))
            *error = "Parsing MetaData failed";
    }
  }
  // Icon
  if (control_dict.Get(keys::kIconKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      std::string icon_path;
      if (!ExtractIconSrc(dict, &icon_path, error)) {
        *error = "Cannot get key value as a dictionary. Key name: icon";
      }
      uiapplicationinfo->app_icons.AddIcon(ApplicationIcon(icon_path));
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          if (!ParseAppIcon(dict, uiapplicationinfo, error))
            *error = "Parsing MetaData failed";
    }
  }
  // Label
  if (control_dict.Get(keys::kLableKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseLabel(dict, uiapplicationinfo))
          *error = "Parsing Label failed";
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          if (!ParseLabel(dict, uiapplicationinfo))
            *error = "Parsing Label failed";
    }
  }
}

}  // namespace

bool UIApplicationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<UIApplicationInfoList>
                  uiapplicationinfo(new UIApplicationInfoList());
  UIApplicationInfoList* normal_ptr = uiapplicationinfo.get();
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kUIApplicationKey, &value))
    return true;

  if (value->GetType() == parser::Value::TYPE_LIST) {
    // multiple entries
    const parser::ListValue* list;
    value->GetAsList(&list);
    int i = 0;
    for (const auto& item : *list) {
      const parser::DictionaryValue* control_dict;
      if (!item->GetAsDictionary(&control_dict)) {
        *error = "Parsing ui-application element failed";
        return false;
      }

      ParseUIApplicationAndStore(*control_dict, &normal_ptr->items[i], error);
      ++i;
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    // single entry
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);

    ParseUIApplicationAndStore(*dict, &normal_ptr->items[0], error);
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
    if (item.ui_info.appid().empty()) {
      *error =
          "The appid child element "
          "of ui-application element is obligatory";
      return false;
    }

    const std::string& exec = item.ui_info.exec();
    if (exec.empty()) {
      *error =
          "The exec child element of ui-application element is obligatory";
      return false;
    }

    const std::string& multiple = item.ui_info.multiple();
    if (multiple.empty()) {
      *error = "The multiple child element of "
               "ui-application element is obligatory";
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
  // AppControl validation
  for (const auto& it : elements.items) {
    for (const auto& item : it.app_control) {
      if (item.operation().empty()) {
        *error =
            "The operation child element of app-control element is obligatory";
        return false;
      }

      const std::string& uri = item.uri();
      if (uri.empty()) {
        *error =
            "The uri child element of app-control element is obligatory";
        return false;
      }

      if (!parser::utils::IsValidIRI(uri)) {
        *error =
            "The uri child element of app-control element is not valid url";
        return false;
      }

      const std::string& mime = item.mime();
      if (mime.empty()) {
        *error =
            "The mime child element of app-control element is obligatory";
        return false;
      }
    }
  }
  // DataControl validation
  for (const auto& it : elements.items) {
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
  }
  // MetaData validation
  for (const auto& it : elements.items) {
    for (const auto& item : it.meta_data) {
      if (item.key().empty()) {
        *error =
            "The key child element of metadata element is obligatory";
        return false;
      }

      const std::string& val = item.val();
      if (val.empty()) {
        *error =
            "The val child element of metadata element is obligatory";
        return false;
      }
    }
  }
  // Label validation
  for (const auto& it : elements.items) {
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
  }
  }
  return true;
}

std::string UIApplicationHandler::Key() const {
  return keys::kUIApplicationKey;
}

}   // namespace parse
}   // namespace tpk
