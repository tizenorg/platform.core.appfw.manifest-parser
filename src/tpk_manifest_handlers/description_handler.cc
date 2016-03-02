// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/description_handler.h"

#include <cassert>
#include <map>
#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/utils/iri_util.h"
#include "manifest_parser/utils/language_tag_validator.h"
#include "manifest_parser/utils/logging.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace keys = tpk::application_keys;

namespace {

const char kDescriptionKey[] = "manifest.description";
const char kDescriptionTextKey[] = "#text";
const char kDescriptionLangKey[] = "@lang";

void ParseDescriptionAndStore(
    const parser::DictionaryValue& description_dict,
    tpk::parse::DescriptionInfoList* descriptioninfo) {
  std::string description;
  description_dict.GetString(kDescriptionTextKey, &description);
  std::string xml_lang;
  description_dict.GetString(kDescriptionLangKey, &xml_lang);

  // TODO(t.iwanek): do not skip it when preloaded apps will be fixed.
  if (description.empty())
    return;

  descriptioninfo->descriptions.emplace_back(description, xml_lang);
}

}  // namespace

namespace tpk {
namespace parse {

DescriptionInfo::DescriptionInfo(
    const std::string& description, const std::string& xml_lang)
    : description_(description),
      xml_lang_(xml_lang) {
}

bool DescriptionHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* /*error*/) {
  std::shared_ptr<DescriptionInfoList>
    descriptioninfo(new DescriptionInfoList());

  for (auto dict : parser::GetOneOrMany(manifest.value(), kDescriptionKey,
                                        {})) {
    ParseDescriptionAndStore(*dict, descriptioninfo.get());
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

  for (const auto& element : elements.descriptions) {
    if (!element.xml_lang().empty() &&
        !utils::w3c_languages::ValidateLanguageTag(element.xml_lang())) {
      *error = "The decription xml:lang failed to validate";
      return false;
    }
  }
  return true;
}

std::string DescriptionInfoList::Key() {
  return kDescriptionKey;
}

std::string DescriptionHandler::Key() const {
  return DescriptionInfoList::Key();
}

}   // namespace parse
}   // namespace tpk
