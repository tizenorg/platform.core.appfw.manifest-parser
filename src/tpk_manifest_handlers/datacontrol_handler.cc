// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/datacontrol_handler.h"

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

void ParseDataControlAndStore(
    const parser::DictionaryValue& control_dict,
    DataControlInfoList* datacontrolinfo) {

  std::string access;
  const parser::DictionaryValue* access_dict;
  if (control_dict.GetDictionary(keys::kDataControlAccessKey,
      &access_dict)) {
    access_dict->GetString(
        keys::kDataControlTypeChildNameAttrKey, &access);
  }

  std::string providerid;
  const parser::DictionaryValue* providerid_dict;
  if (control_dict.GetDictionary(
      keys::kDataControlProviderIDKey,
      &providerid_dict)) {
    providerid_dict->GetString(
        keys::kDataControlTypeChildNameAttrKey, &providerid);
  }

  std::string type;
  const parser::DictionaryValue* type_dict;
  if (control_dict.GetDictionary(
      keys::kDataControlTypeKey,
      &type_dict)) {
    type_dict->GetString(
        keys::kDataControlTypeChildNameAttrKey, &type);
  }

  datacontrolinfo->items.emplace_back(access, providerid, type);
}

}  // namespace

bool DataControlHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<DataControlInfoList>
    datacontrolinfo(new DataControlInfoList());
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kDataControlKey, &value))
    return true;

  if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);
    ParseDataControlAndStore(*dict, datacontrolinfo.get());
  } else {
    *error = "Cannot parse datacontrol element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(datacontrolinfo);
  return true;
}

bool DataControlHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const DataControlInfoList& elements =
       static_cast<const DataControlInfoList&>(data);

  for (const auto& item : elements.items) {
    if (item.access().empty()) {
      *error =
          "The access child element of datacontrol element is obligatory";
      return false;
    }

    const std::string& providerid = item.providerid();
    if (providerid.empty()) {
      *error =
          "The providerid child element of datacontrol element is obligatory";
      return false;
    }

    const std::string& type = item.type();
    if (type.empty()) {
      *error =
          "The type child element of datacontrol element is obligatory";
      return false;
    }
  }
  return true;
}

std::string DataControlHandler::Key() const {
  return keys::kDataControlKey;
}

DataControlInfo::DataControlInfo(
    const std::string& access,
    const std::string& providerid,
    const std::string& type)
    : access_(access),
      providerid_(providerid),
      type_(type) {}

}   // namespace parse
}   // namespace tpk
