// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/account_handler.h"

#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "utils/language_tag_validator.h"
#include "utils/logging.h"

namespace {
const char kTrueValueString[] = "true";
const char kFalseValueString[] = "false";
}

namespace tpk {
namespace parse {

namespace keys = tpk::application_keys;

bool AccountHandler::Parse(const parser::Manifest& manifest,
                           std::shared_ptr<parser::ManifestData>* output,
                           std::string* error) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  std::shared_ptr<AccountInfo> info(new AccountInfo);

  if (manifest.Get(keys::kAccountKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseSingleAccountElement(dict, info, error))
        return false;
    } else if (val->GetAsList(&list)) {
      *error = "<account> may be declared only once";
      return false;
    }
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

  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  if (item_dict->Get(keys::kAccountProviderKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (!ParseAccountProvider(dict, info, error))
          return false;
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          if (!ParseAccountProvider(dict, info, error))
            return false;
    }
  }
  return true;
}

bool AccountHandler::ParseAccountProvider(
  const parser::DictionaryValue* dict,
  std::shared_ptr<AccountInfo> info,
  std::string* error) {
  SingleAccountInfo single_account;
  std::string app_id;
  if (!dict->GetString(keys::kAccountAppIDKey, &app_id)) {
    *error = "Error while parsing app id in account";
    return false;
  }

  single_account.app_id = app_id;

  std::string multiple_apps_support;
  if (!dict->GetString(keys::kAccountMASKey, &multiple_apps_support)) {
    *error = "Error while parsing multiple accounts support in account";
    return false;
  }

  std::string provider_id;
  if (!dict->GetString(keys::kAccountProviderIDKey, &provider_id)) {
    *error = "Error while parsing provider id in account";
    return false;
  }
  single_account.provider_id = provider_id;

  bool mas_bool = false;
  if (multiple_apps_support == kFalseValueString) {
    mas_bool = false;
  } else if (multiple_apps_support == kTrueValueString) {
    mas_bool = true;
  } else {
    *error = "Multiple accounts support in "
             "account has invalid (not boolean) value";
    return false;
  }

  single_account.multiple_account_support = mas_bool;
  if (!ParseAccountIcons(dict, &single_account)) {
    *error = "Error while parsing account icons";
    return false;
  }
  if (!ParseLabels(dict, &single_account)) {
    *error = "Error while parsing account lables";
    return false;
  }
  if (!ParseCapabilities(dict, &single_account)) {
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
  if (section.compare(keys::kAccountIconNormalKey) != 0 &&
      section.compare(keys::kAccountIconSmallKey) != 0)
    return false;
  std::string icon_path;
  item_dict->GetString(keys::kAccountTextKey, &icon_path);
  info->icon_paths.push_back(std::make_pair(section, icon_path));
  return true;
}

bool AccountHandler::ParseLabels(
    const parser::DictionaryValue* item_dict,
    SingleAccountInfo* info) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  std::string string_value;
  if (item_dict->Get(keys::kAccountLabelKey, &val)) {
    std::string label;
    std::string lang;
    if (val->GetAsDictionary(&dict)) {
      dict->GetString(keys::kAccountLangKey, &lang);
      dict->GetString(keys::kAccountNameKey, &label);
      info->labels.push_back(std::make_pair(label, lang));
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict)) {
          dict->GetString(keys::kAccountLangKey, &lang);
          dict->GetString(keys::kAccountNameKey, &label);
          info->labels.push_back(std::make_pair(label, lang));
        }
    }
  }
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
}  // namespace tpk
