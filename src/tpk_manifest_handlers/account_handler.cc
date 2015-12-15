// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/account_handler.h"

#include "../manifest_parser/utils/language_tag_validator.h"
#include "../manifest_parser/utils/logging.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace {
const char kTrueValueString[] = "true";
const char kFalseValueString[] = "false";
}

namespace tpk {
namespace parse {

namespace keys = tpk::application_keys;

namespace {
const char kAccountKey[] = "manifest.account";
const char kAccountProviderKey[] = "account-provider";
const char kAccountProviderIDKey[] = "@providerid";
const char kAccountAppIDKey[] = "@appid";
const char kAccountMASKey[] = "@multiple-accounts-support";
const char kAccountIconKey[] = "icon";
const char kAccountSectionKey[] = "@section";
const char kAccountTextKey[] = "#text";
const char kAccountLabelKey[] = "label";
const char kAccountCapabilityKey[] = "capability";
const char kAccountLangKey[] = "@lang";
const char kAccountIconNormalKey[] = "account";
const char kAccountIconSmallKey[] = "account-small";
}  // namespace

bool AccountHandler::Parse(const parser::Manifest& manifest,
                           std::shared_ptr<parser::ManifestData>* output,
                           std::string* error) {
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  std::shared_ptr<AccountInfo> info(new AccountInfo);

  if (manifest.Get(kAccountKey, &val)) {
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

std::string AccountInfo::key() {
  return kAccountKey;
}

std::string AccountHandler::Key() const {
  return kAccountKey;
}

bool AccountHandler::ParseSingleAccountElement(
    const parser::DictionaryValue* item_dict,
    std::shared_ptr<AccountInfo> info,
    std::string* error) {
  for (auto& item : parser::GetOneOrMany(item_dict, kAccountProviderKey, "")) {
    if (!ParseAccountProvider(item, info, error))
      return false;
  }
  return true;
}

bool AccountHandler::ParseAccountProvider(
  const parser::DictionaryValue* dict,
  std::shared_ptr<AccountInfo> info,
  std::string* error) {
  SingleAccountInfo single_account;
  std::string app_id;
  if (!dict->GetString(kAccountAppIDKey, &app_id)) {
    *error = "Error while parsing app id in account";
    return false;
  }

  single_account.app_id = app_id;

  std::string multiple_apps_support;
  if (!dict->GetString(kAccountMASKey, &multiple_apps_support)) {
    *error = "Error while parsing multiple accounts support in account";
    return false;
  }

  std::string provider_id;
  if (!dict->GetString(kAccountProviderIDKey, &provider_id)) {
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
  auto& icons = parser::GetOneOrMany(item_dict, kAccountIconKey, "");
  if (icons.empty())
    return false;
  for (auto& item : icons) {
    if (!ParseSingleAccountIcon(item, info))
      return false;
  }
  return true;
}

bool AccountHandler::ParseSingleAccountIcon(
    const parser::DictionaryValue* item_dict,
    SingleAccountInfo* info) {
  std::string section;
  item_dict->GetString(kAccountSectionKey, &section);
  if (section != kAccountIconNormalKey &&
      section != kAccountIconSmallKey)
    return false;
  std::string icon_path;
  item_dict->GetString(kAccountTextKey, &icon_path);
  info->icon_paths.push_back(std::make_pair(section, icon_path));
  return true;
}

bool AccountHandler::ParseLabels(
    const parser::DictionaryValue* item_dict,
    SingleAccountInfo* info) {
  for (auto& item : parser::GetOneOrMany(item_dict, kAccountLabelKey, "")) {
    std::string lang;
    std::string label;
    item->GetString(kAccountLangKey, &lang);
    item->GetString(kAccountTextKey, &label);
    info->labels.push_back(std::make_pair(label, lang));
  }
  return true;
}

bool AccountHandler::ParseCapabilities(
    const parser::DictionaryValue* item_dict,
    SingleAccountInfo* info) {
  for (auto& item : parser::GetOneOrMany(item_dict, kAccountCapabilityKey,
                                         "")) {
    std::string capability;
    item->GetString(kAccountTextKey, &capability);
    info->capabilities.push_back(capability);
  }
  return true;
}

}  // namespace parse
}  // namespace tpk
