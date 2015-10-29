// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/background_category_handler.h"

#include "utils/logging.h"

namespace {
const char kTizenBackgroundCategoryValueKey[] = "@value";
const char kErrMsgElementParse[] = "Parsing background-category element failed";
}

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

bool BackgroundCategoryHandler::ParseBackgroundCategoryElement(
    const parser::DictionaryValue& element_dict,
    BackgroundCategoryInfoList* bclist) {
  std::string value;

  if (!element_dict.GetString(kTizenBackgroundCategoryValueKey, &value))
    return false;

  bclist->background_categories.emplace_back(&value);

  return true;
}

bool BackgroundCategoryHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  const parser::Value* value = nullptr;
  std::shared_ptr<BackgroundCategoryInfoList> bclist(
      new BackgroundCategoryInfoList());

  if (!manifest.Get(keys::kBackgroundCategoryKey, &value))
    return true;

  if (value->GetType() == parser::Value::TYPE_LIST) {
    const parser::ListValue* list;

    value->GetAsList(&list);

    for (const auto& item : *list) {
      const parser::DictionaryValue* control_dict;

      if (!item->GetAsDictionary(&control_dict))
        continue;

      if (!parser::VerifyElementNamespace(
          *control_dict, keys::kTizenNamespacePrefix))
        continue;

      if (!ParseBackgroundCategoryElement(*control_dict, bclist.get())) {
        *error = kErrMsgElementParse;
        return nullptr;
      }
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict;

    value->GetAsDictionary(&dict);

    if (!parser::VerifyElementNamespace(
        *dict, keys::kTizenNamespacePrefix))
      return nullptr;

    if (!ParseBackgroundCategoryElement(*dict, bclist.get()))
      return nullptr;
  } else {
    LOG(INFO) << "Background-category element is not defined";
    return true;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(bclist);
}

std::string BackgroundCategoryHandler::Key() const {
  return keys::kBackgroundCategoryKey;
}

BackgroundCategoryInfo::BackgroundCategoryInfo(const std::string& value) :
    value_(value) {}

} // namespace parse
} // namespace wgt
