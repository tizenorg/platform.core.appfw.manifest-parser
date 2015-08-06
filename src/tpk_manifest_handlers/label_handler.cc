// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/label_handler.h"

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

void ParseLabelAndStore(
    const parser::DictionaryValue& control_dict,
    LabelInfoList* labelinfo) {

  std::string text;
  const parser::DictionaryValue* text_dict;
  if (control_dict.GetDictionary(keys::kLableKey,
      &text_dict)) {
    text_dict->GetString(
        keys::kLableKeyText, &text);
  }

  std::string name;
  const parser::DictionaryValue* name_dict;
  if (control_dict.GetDictionary(keys::kLableKey,
      &name_dict)) {
    name_dict->GetString(
        keys::kLableKeyName, &name);
  }

  std::string xml_lang;
  const parser::DictionaryValue* xml_lang_dict;
  if (control_dict.GetDictionary(keys::kLableLangKey,
      &xml_lang_dict)) {
    xml_lang_dict->GetString(
        keys::kLableKeyText, &xml_lang);
  }

  labelinfo->items.emplace_back(text, name, xml_lang);
}

}  // namespace

bool LabelHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<LabelInfoList> labelinfo(new LabelInfoList());
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kLableKey, &value))
    return true;

  if (value->GetType() == parser::Value::TYPE_LIST) {
    // multiple entries
    const parser::ListValue* list;
    value->GetAsList(&list);
    for (const auto& item : *list) {
      const parser::DictionaryValue* control_dict;
      if (!item->GetAsDictionary(&control_dict)) {
        *error = "Parsing label element failed";
        return false;
      }

      ParseLabelAndStore(*control_dict, labelinfo.get());
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    // single entry
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);
    ParseLabelAndStore(*dict, labelinfo.get());
  } else {
    *error = "Cannot parse label element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(labelinfo);
  return true;
}

bool LabelHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const LabelInfoList& elements =
       static_cast<const LabelInfoList&>(data);

  for (const auto& item : elements.items) {
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
  return true;
}

std::string LabelHandler::Key() const {
  return keys::kLableKey;
}

}   // namespace parse
}   // namespace tpk
