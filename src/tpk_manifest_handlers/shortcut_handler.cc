// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/shortcut_handler.h"

#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "utils/language_tag_validator.h"

namespace keys = tpk::application_keys;

namespace {

const char kShortcutListKey[] = "manifest.shortcut-list";
const char kShortcutKey[] = "shortcut";
const char kShortcutAppidKey[] = "@appid";
const char kShortcutExtraDataKey[] = "@extra_data";
const char kShortcutExtraKeyKey[] = "@extra_key";
const char kShortcutIconKey[] = "icon";
const char kShortcutIconTextKey[] = "#text";
const char kShortcutLabelKey[] = "label";
const char kShortcutLabelTextKey[] = "#text";
const char kShortcutLabelLangKey[] = "@lang";

bool ParseShortcutLabel(const parser::DictionaryValue* dict,
                        std::string* error,
                        std::pair<std::string, std::string>* label) {
  std::string lang;
  if (dict->GetString(kShortcutLabelLangKey, &lang)) {
    if (!utils::w3c_languages::ValidateLanguageTag(lang)) {
      *error = "Failed to validate language tag of shortcut's label element";
      return false;
    }
  }
  label->first = lang;
  dict->GetString(kShortcutLabelTextKey, &label->second);
  return true;
}

bool ParseShortcut(const parser::DictionaryValue* dict, std::string* error,
                   tpk::parse::ShortcutListInfo* list) {
  tpk::parse::ShortcutInfo shortcut;
  dict->GetString(kShortcutAppidKey, &shortcut.app_id);
  dict->GetString(kShortcutExtraDataKey, &shortcut.extra_data);
  dict->GetString(kShortcutExtraKeyKey, &shortcut.extra_key);

  for (auto& item : parser::GetOneOrMany(dict, kShortcutLabelKey, "")) {
    std::pair<std::string, std::string> label;
    if (!ParseShortcutLabel(item, error, &label))
      return false;
    shortcut.labels.push_back(label);
  }

  const parser::Value* icon_value = nullptr;
  if (dict->Get(kShortcutIconKey, &icon_value)) {
    const parser::DictionaryValue* icon_dict = nullptr;
    if (!icon_value->GetAsDictionary(&icon_dict)) {
      *error = "Invalid shortcut icon. Parser is expecting single <icon> tag";
      return false;
    }
    icon_dict->GetString(kShortcutIconTextKey, &shortcut.icon);
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
  if (!manifest.Get(kShortcutListKey, &listvalue))
    return true;
  const parser::DictionaryValue* listdict = nullptr;
  if (!listvalue->GetAsDictionary(&listdict)) {
    *error = "Cannot parse shortcut-list element. Single element is expected";
    return false;
  }
  std::shared_ptr<ShortcutListInfo> shortcuts(new ShortcutListInfo());
  for (auto& item : parser::GetOneOrMany(listdict, kShortcutKey, "")) {
    if (!ParseShortcut(item, error, shortcuts.get()))
      return false;
  }
  *output = std::static_pointer_cast<parser::ManifestData>(shortcuts);
  return true;
}

std::string ShortcutListInfo::key() {
  return kShortcutListKey;
}

std::string ShortcutHandler::Key() const {
  return kShortcutListKey;
}

}  // namespace parse
}  // namespace tpk
