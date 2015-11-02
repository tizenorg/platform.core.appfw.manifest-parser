// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "manifest_handlers/account_handler.h"

#include "manifest_handlers/application_manifest_constants.h"
#include "utils/language_tag_validator.h"
#include "utils/logging.h"

namespace {
const char kSectionIconAccount[] = "Account";
const char kSectionIconAccountSmall[] = "AccountSmall";
const char kTrueValueString[] = "true";
const char kFalseValueString[] = "false";
}

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

bool AccountHandler::Parse(const parser::Manifest& manifest,
                           std::shared_ptr<parser::ManifestData>* output,
                           std::string* error) {
  if (!manifest.HasPath(keys::kAccountKey))
    return true;

  std::shared_ptr<AccountInfo> info(new AccountInfo);

  for (const auto& dict : parser::GetOneOrMany(manifest.value(),
      keys::kAccountKey, "")) {
    if (!ParseSingleAccountElement(dict, info, error))
      return false;
  }

  *output = std::static_pointer_cast<AccountInfo>(info);
  return true;
}

std::string AccountHandler::Key() const {
  return keys::kAccountKey;
}

bool AccountHandler::ParseSingleAccountElement(
    const parser::DictionaryValue* item_dict,
    std::shared_ptr<AccountInfo> info,
    std::string* error) {
  std::string multiple_apps_support;
  SingleAccountInfo single_account;
  if (!item_dict->GetString(keys::kAccountMASKey, &multiple_apps_support)) {
    *error = "Error while parsing multiple apps support in account";
    return false;
  }
  bool mas_bool = false;
  if (multiple_apps_support == kFalseValueString) {
    mas_bool = false;
  } else if (multiple_apps_support == kTrueValueString) {
    mas_bool = true;
  } else {
    *error = "Multiple apps support in account has invalid (not boolean) value";
    return false;
  }
  single_account.multiple_account_support = mas_bool;
  if (!ParseAccountNames(item_dict, &single_account)) {
    *error = "Error while parsing account names";
    return false;
  }
  if (!ParseAccountIcons(item_dict, &single_account)) {
    *error = "Error while parsing account icons";
    return false;
  }
  if (!ParseCapabilities(item_dict, &single_account)) {
    *error = "Error while parsing account capabilities";
    return false;
  }
  info->set_account(single_account);
  return true;
}

bool AccountHandler::ParseAccountIcons(
  const parser::DictionaryValue* item_dict,
  SingleAccountInfo* info) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  if (item_dict->Get(keys::kAccountIconKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseSingleAccountIcon(dict, info))
        return false;
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict) &&
            !ParseSingleAccountIcon(dict, info))
          return false;
    }
    return true;
  }
  return false;
}

bool AccountHandler::ParseSingleAccountIcon(
    const parser::DictionaryValue* item_dict,
    SingleAccountInfo* info) {
  std::string section;
  item_dict->GetString(keys::kAccountSectionKey, &section);
  if (section.compare(kSectionIconAccount) != 0 &&
      section.compare(kSectionIconAccountSmall) != 0)
    return false;
  std::string icon_path;
  item_dict->GetString(keys::kAccountTextKey, &icon_path);
  info->icon_paths.push_back(std::make_pair(section, icon_path));
  return true;
}

bool AccountHandler::ParseAccountNames(
  const parser::DictionaryValue* item_dict,
  SingleAccountInfo* info) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  if (item_dict->Get(keys::kAccountNameKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseSingleAccountName(dict, info))
        return false;
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict) &&
            !ParseSingleAccountName(dict, info))
          return false;
    }
    return true;
  }
  return false;
}

bool AccountHandler::ParseSingleAccountName(
    const parser::DictionaryValue* item_dict,
    SingleAccountInfo* info) {
  std::string lang;
  if (item_dict->GetString(keys::kAccountLangKey, &lang) &&
      !utils::w3c_languages::ValidateLanguageTag(lang)) {
    return false;
  }
  std::string name;
  item_dict->GetString(keys::kAccountTextKey, &name);
  info->names.push_back(std::make_pair(name, lang));
  return true;
}


bool AccountHandler::ParseCapabilities(
    const parser::DictionaryValue* item_dict,
    SingleAccountInfo* info) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  std::string string_value;
  if (item_dict->Get(keys::kAccountCapabilityKey, &val)) {
    std::string capability;
    if (val->GetAsDictionary(&dict)) {
      dict->GetString(keys::kAccountTextKey, &capability);
      info->capabilities.push_back(capability);
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict)) {
          dict->GetString(keys::kAccountTextKey, &capability);
          info->capabilities.push_back(capability);
        }
    }
  }
  return true;
}

}  // namespace parse
}  // namespace wgt
