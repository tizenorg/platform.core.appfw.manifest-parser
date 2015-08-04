// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/description_handler.h"

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

void ParseDescriptionAndStore(
    const parser::DictionaryValue& control_dict,
    DescriptionInfoList* descriptioninfo) {

  std::string description;
  const parser::DictionaryValue* description_dict;
  if (control_dict.GetDictionary(keys::kDescriptionKey,
                                 &description_dict)) {
    description_dict->GetString(
        keys::kDescriptionKeyText, &description);
  }

  std::string xml_lang;
  const parser::DictionaryValue* xml_lang_dict;
  if (control_dict.GetDictionary(keys::kDescriptionLangKey,
                                 &xml_lang_dict)) {
    xml_lang_dict->GetString(
        keys::kDescriptionKeyText, &xml_lang);
  }

  descriptioninfo->items.emplace_back(description, xml_lang);
}

}  // namespace

bool DescriptionHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<DescriptionInfoList>
    descriptioninfo(new DescriptionInfoList());
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kDescriptionKey, &value))
    return true;

  if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);
    ParseDescriptionAndStore(*dict, descriptioninfo.get());
  } else {
    *error = "Cannot parse description element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(descriptioninfo);
  return true;
}

bool DescriptionHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const DescriptionInfoList& elements =
       static_cast<const DescriptionInfoList&>(data);

  for (const auto& item : elements.items) {
    if (item.description().empty()) {
      *error =
          "The description child element of description element is obligatory";
      return false;
    }
  }
  return true;
}

std::string DescriptionHandler::Key() const {
  return keys::kDescriptionKey;
}

}   // namespace parse
}   // namespace tpk
