// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/metadata_handler.h"

#include <cassert>
#include <map>
#include <utility>

#include "utils/logging.h"
#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_parser/values.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

typedef std::pair<std::string, std::string> MetaDataPair;
typedef std::map<std::string, std::string> MetaDataMap;
typedef std::map<std::string, std::string>::const_iterator MetaDataIter;

namespace {

const char kWidgetNamespacePrefix[] = "http://www.w3.org/ns/widgets";
const char kTizenNamespacePrefix[] = "http://tizen.org/ns/widgets";
const char kTizenMetaDataNameKey[] = "@key";
const char kTizenMetaDataValueKey[] = "@value";
const char kTizenMetaDataKey[] = "widget.metadata";

MetaDataPair ParseMetaDataItem(const parser::DictionaryValue* dict,
                               std::string* error) {
  assert(dict && dict->IsType(parser::Value::TYPE_DICTIONARY));
  MetaDataPair result;
  if (!dict->GetString(kTizenMetaDataNameKey, &result.first)) {
    *error = "Invalid key of tizen metaData.";
  } else {
    if (!dict->GetString(kTizenMetaDataValueKey, &result.second)) {
      result.second = "";
    }
  }

  return result;
}

}  // namespace

MetaDataInfo::MetaDataInfo() {}

MetaDataInfo::~MetaDataInfo() {}

bool MetaDataInfo::HasKey(const std::string& key) const {
  return metadata_.find(key) != metadata_.end();
}

std::string MetaDataInfo::GetValue(const std::string& key) const {
  MetaDataIter it = metadata_.find(key);
  if (it != metadata_.end()) return it->second;
  return std::string("");
}

void MetaDataInfo::SetValue(const std::string& key, const std::string& value) {
  metadata_.insert(MetaDataPair(key, value));
}

MetaDataHandler::MetaDataHandler() {}

MetaDataHandler::~MetaDataHandler() {}

bool MetaDataHandler::Parse(const parser::Manifest& manifest,
                            std::shared_ptr<parser::ManifestData>* output,
                            std::string* error) {
  std::shared_ptr<MetaDataInfo> metadata_info(new MetaDataInfo);
  parser::Value* metadata_value = nullptr;

  if (!manifest.Get(kTizenMetaDataKey, &metadata_value)) {
    LOG(INFO) << "Failed to get value of tizen metaData";
    return true;
  }

  MetaDataPair metadata_item;
  if (metadata_value &&
      metadata_value->IsType(parser::Value::TYPE_DICTIONARY)) {
    parser::DictionaryValue* dict;
    metadata_value->GetAsDictionary(&dict);
    if (parser::VerifyElementNamespace(*dict, kTizenNamespacePrefix)) {
      metadata_item = ParseMetaDataItem(dict, error);
      metadata_info->SetValue(metadata_item.first, metadata_item.second);
    }
  } else if (metadata_value &&
             metadata_value->IsType(parser::Value::TYPE_LIST)) {
    parser::ListValue* list;
    metadata_value->GetAsList(&list);

    for (parser::ListValue::iterator it = list->begin(); it != list->end();
         ++it) {
      parser::DictionaryValue* dict;
      (*it)->GetAsDictionary(&dict);
      if (!parser::VerifyElementNamespace(*dict, kTizenNamespacePrefix))
        continue;
      metadata_item = ParseMetaDataItem(dict, error);
      metadata_info->SetValue(metadata_item.first, metadata_item.second);
    }
  } else {
    *error =
        "tizen metaData element exists and its type is neither "
        "TYPE_LIST nor TYPE_DICTIONARY.";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(metadata_info);
  return true;
}

bool MetaDataHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* /*error*/) const {
  const MetaDataInfo& mdata_info = static_cast<const MetaDataInfo&>(data);
  // TODO(j.izydorczyk):
  // Here should be performed *info class members validity check
  (void)mdata_info;
  return true;
}

std::string MetaDataHandler::Key() const { return kTizenMetaDataKey; }

std::string MetaDataInfo::key() {
  return kTizenMetaDataKey;
}


}  // namespace parse
}  // namespace wgt
