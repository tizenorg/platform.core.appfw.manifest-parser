// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/common/application_handler.h"

namespace tpk {
namespace parse {
namespace app_keys {
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
}  // namespace app_keys

void ApplicationInfo::set_appid(const std::string& appid) {
  appid_ = appid;
}

void ApplicationInfo::set_exec(const std::string& exec) {
  exec_ = exec;
}

void ApplicationInfo::set_type(const std::string& type) {
  type_ = type;
}
void ApplicationInfo::set_process_pool(const std::string& process_pool) {
  process_pool_ = process_pool;
}

const std::string& ApplicationInfo::appid() const {
  return appid_;
}

const std::string& ApplicationInfo::exec() const {
  return exec_;
}

const std::string& ApplicationInfo::type() const {
  return type_;
}
const std::string& ApplicationInfo::process_pool() const {
  return process_pool_;
}

std::string GetAppControlValue(const parser::DictionaryValue& dict,
                               std::string key) {
  std::string value;
  const parser::DictionaryValue* operation_dict;

  if (dict.GetDictionary(key, &operation_dict)) {
    operation_dict->GetString(app_keys::kAppControlNameChildKey, &value);
  }

  return value;
}

}  // namespace parse
}  // namespace tpk
