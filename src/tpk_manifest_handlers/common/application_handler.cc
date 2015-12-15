// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/common/application_handler.h"

namespace tpk {
namespace parse {
namespace tpk_app_keys {
// app-control
const char kAppControlKey[] = "app-control";
const char kAppControlOperationKey[] = "operation";
const char kAppControlURIKey[] = "uri";
const char kAppControlMimeKey[] = "mime";
const char kAppControlNameChildKey[] = "@name";

// background-category
const char kBackgroundCategoryKey[] = "background-category";
const char kBackgroundCategoryValueKey[] = "@value";

// datacontrol
const char kDataControlKey[] = "datacontrol";
const char kDataControlAccessKey[] = "@access";
const char kDataControlProviderIDKey[] = "@providerid";
const char kDataControlTypeKey[] = "@type";

// icon
const char kIconKey[] = "icon";
const char kIconTextKey[] = "#text";

// label
const char kLabelKey[] = "label";
const char kLabelTextKey[] = "#text";
const char kLabelLangKey[] = "@lang";

// metadata
const char kMetaDataKey[] = "metadata";
const char kMetaDataKeyKey[] = "@key";
const char kMetaDataValueKey[] = "@value";
}  // namespace tpk_app_keys

const utils::VersionNumber kLaunchModeRequiredVersion("2.4");

std::string GetAppControlValue(const parser::DictionaryValue& dict,
                               const std::string& key) {
  std::string value;
  const parser::DictionaryValue* inner_dict;

  if (dict.GetDictionary(key, &inner_dict)) {
    inner_dict->GetString(tpk_app_keys::kAppControlNameChildKey, &value);
  }

  return value;
}

}  // namespace parse
}  // namespace tpk
