// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/shortcut_handler.h"

#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "utils/language_tag_validator.h"

namespace keys = tpk::application_keys;

namespace {

bool ParseShortcutLabel(const parser::DictionaryValue* dict,
                        std::string* error,
                        std::pair<std::string, std::string>* label) {
  std::string lang;
  if (dict->GetString(keys::kShortcutLabelLangKey, &lang)) {
    if (!utils::w3c_languages::ValidateLanguageTag(lang)) {
      *error = "Failed to validate language tag of shortcut's label element";
      return false;
    }
  }
  label->first = lang;
  dict->GetString(keys::kShortcutLabelTextKey, &label->second);
  return true;
}

bool ParseShortcut(const parser::DictionaryValue* dict, std::string* error,
                   tpk::parse::ShortcutListInfo* list) {
  tpk::parse::ShortcutInfo shortcut;
  dict->GetString(keys::kShortcutAppidKey, &shortcut.app_id);
  dict->GetString(keys::kShortcutExtraDataKey, &shortcut.extra_data);
  dict->GetString(keys::kShortcutExtraKeyKey, &shortcut.extra_key);

  const parser::Value* labels_value = nullptr;
  if (dict->Get(keys::kShortcutLabelKey, &labels_value)) {
    if (labels_value->GetType() == parser::Value::TYPE_DICTIONARY) {
      const parser::DictionaryValue* ldict = nullptr;
      labels_value->GetAsDictionary(&ldict);
      std::pair<std::string, std::string> label;
      if (!ParseShortcutLabel(ldict, error, &label))
        return false;
      shortcut.labels.push_back(label);
    } else if (labels_value->GetType() == parser::Value::TYPE_LIST) {
      const parser::ListValue* list = nullptr;
      labels_value->GetAsList(&list);
      for (auto& item : *list) {
        const parser::DictionaryValue* ldict = nullptr;
        if (!item->GetAsDictionary(&ldict))
          continue;
        std::pair<std::string, std::string> label;
        if (!ParseShortcutLabel(ldict, error, &label))
          return false;
        shortcut.labels.push_back(label);
      }
    } else {
      *error = "Invalid value of shortcut's label element";
      return false;
    }
  }
  const parser::Value* icon_value = nullptr;
  if (dict->Get(keys::kShortcutIconKey, &icon_value)) {
    const parser::DictionaryValue* icon_dict = nullptr;
    if (!icon_value->GetAsDictionary(&icon_dict)) {
      *error = "Invalid shortcut icon. Parser is expecting single <icon> tag";
      return false;
    }
    icon_dict->GetString(keys::kShortcutIconTextKey, &shortcut.icon);
  }

  list->shortcuts.push_back(shortcut);
  return true;
}

}  // namespace

namespace tpk {
namespace parse {

bool ShortcutHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  const parser::Value* listvalue = nullptr;
  if (!manifest.Get(keys::kShortcutListKey, &listvalue))
    return true;
  const parser::DictionaryValue* listdict = nullptr;
  if (!listvalue->GetAsDictionary(&listdict)) {
    *error = "Cannot parse shortcut-list element. Single element is expected";
    return false;
  }
  const parser::Value* value = nullptr;
  if (!listdict->Get(keys::kShortcutKey, &value))
    return false;

  std::shared_ptr<ShortcutListInfo> shortcuts(new ShortcutListInfo());
  if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    if (!ParseShortcut(dict, error, shortcuts.get()))
      return false;
  } else if (value->GetType() == parser::Value::TYPE_LIST) {
    const parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      item->GetAsDictionary(&dict);
      if (!ParseShortcut(dict, error, shortcuts.get()))
        return false;
    }
  } else {
    *error = "Invalid value of shortcut-list element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(shortcuts);
  return true;
}

std::string ShortcutHandler::Key() const {
  return keys::kShortcutListKey;
}

}  // namespace parse
}  // namespace tpk
