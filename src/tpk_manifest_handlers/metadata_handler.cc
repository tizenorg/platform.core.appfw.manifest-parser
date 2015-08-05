// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/metadata_handler.h"

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

void ParseMetaDataAndStore(
    const parser::DictionaryValue& control_dict,
    MetaDataInfoList* metadatainfo) {

  std::string key;
  const parser::DictionaryValue* key_dict;
  if (control_dict.GetDictionary(keys::kMetaDataKey,
                                 &key_dict)) {
    key_dict->GetString(
        keys::kMetaDataTypeChildNameAttrKey, &key);
  }

  std::string val;
  const parser::DictionaryValue* value_dict;
  if (control_dict.GetDictionary(keys::kMetaDataValueKey,
                                 &value_dict)) {
    value_dict->GetString(
        keys::kMetaDataTypeChildNameAttrKey, &val);
  }

  metadatainfo->items.emplace_back(key, val);
}

}  // namespace

bool MetaDataHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<MetaDataInfoList>
    metadatainfo(new MetaDataInfoList());
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kMetaData, &value))
    return true;

  if (value->GetType() == parser::Value::TYPE_LIST) {
    // multiple entries
    const parser::ListValue* list;
    value->GetAsList(&list);
    for (const auto& item : *list) {
      const parser::DictionaryValue* control_dict;
      if (!item->GetAsDictionary(&control_dict)) {
        *error = "Parsing metadata element failed";
        return false;
      }

      ParseMetaDataAndStore(*control_dict, metadatainfo.get());
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    // single entry
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);
    ParseMetaDataAndStore(*dict, metadatainfo.get());
  } else {
    *error = "Cannot parse metadata element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(metadatainfo);
  return true;
}

bool MetaDataHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const MetaDataInfoList& elements =
       static_cast<const MetaDataInfoList&>(data);

  for (const auto& item : elements.items) {
    if (item.key().empty()) {
      *error =
          "The key child element of metadata element is obligatory";
      return false;
    }

    const std::string& val = item.val();
    if (val.empty()) {
      *error =
          "The val child element of metadata element is obligatory";
      return false;
    }
  }
  return true;
}

std::string MetaDataHandler::Key() const {
  return keys::kMetaData;
}

}   // namespace parse
}   // namespace tpk
