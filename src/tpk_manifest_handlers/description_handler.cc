// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/description_handler.h"

#include <cassert>
#include <map>
#include <utility>

#include "manifest_parser/utils/iri_util.h"
#include "manifest_parser/utils/logging.h"
#include "manifest_parser/manifest_util.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

namespace keys = tpk::application_keys;

namespace {
const char kDescriptionKey[] = "description";
const char kDescriptionKeyText[] = "#text";
const char kDescriptionLangKey[] = "@lang";

void ParseDescriptionAndStore(
    const parser::DictionaryValue& control_dict,
    DescriptionInfo* descriptioninfo) {

  std::string description;
  const parser::DictionaryValue* description_dict;
  if (control_dict.GetDictionary(kDescriptionKey,
                                 &description_dict)) {
    description_dict->GetString(
        kDescriptionKeyText, &description);
  }

  std::string xml_lang;
  const parser::DictionaryValue* xml_lang_dict;
  if (control_dict.GetDictionary(kDescriptionLangKey,
                                 &xml_lang_dict)) {
    xml_lang_dict->GetString(
        kDescriptionKeyText, &xml_lang);
  }

  descriptioninfo->set_description(description);
  descriptioninfo->set_xml_lang(xml_lang);
}

}  // namespace

bool DescriptionHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<DescriptionInfo>
    descriptioninfo(new DescriptionInfo());
  parser::Value* value = nullptr;
  if (!manifest.Get(kDescriptionKey, &value))
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
  const DescriptionInfo& elements =
      static_cast<const DescriptionInfo&>(data);

  if (elements.description().empty()) {
    *error =
        "The description child element of description element is obligatory";
    return false;
  }
  return true;
}

std::string DescriptionInfo::key() {
  return kDescriptionKey;
}

std::string DescriptionHandler::Key() const {
  return kDescriptionKey;
}

}   // namespace parse
}   // namespace tpk
