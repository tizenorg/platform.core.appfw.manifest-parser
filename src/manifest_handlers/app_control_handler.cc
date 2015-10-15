// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/app_control_handler.h"

#include "manifest_parser/values.h"
#include "manifest_handlers/application_manifest_constants.h"
#include "utils/iri_util.h"

namespace {
const char kEnabledValue[] = "enabled";
const char kDisabledValue[] = "disabled";
}  // namespace

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

namespace {

void ParseAppControlEntryAndStore(
    const parser::DictionaryValue& control_dict,
    AppControlInfoList* aplist) {
  std::string src;
  const parser::DictionaryValue* src_dict;
  if (control_dict.GetDictionary(keys::kTizenApplicationAppControlSrcKey,
      &src_dict)) {
    src_dict->GetString(
        keys::kTizenApplicationAppControlChildNameAttrKey, &src);
  }

  std::string operation;
  const parser::DictionaryValue* operation_dict;
  if (control_dict.GetDictionary(
      keys::kTizenApplicationAppControlOperationKey,
      &operation_dict)) {
    operation_dict->GetString(
        keys::kTizenApplicationAppControlChildNameAttrKey, &operation);
  }

  std::string uri;
  const parser::DictionaryValue* uri_dict;
  if (control_dict.GetDictionary(keys::kTizenApplicationAppControlUriKey,
      &uri_dict)) {
    uri_dict->GetString(
        keys::kTizenApplicationAppControlChildNameAttrKey, &uri);
  }

  std::string mime;
  const parser::DictionaryValue* mime_dict;
  if (control_dict.GetDictionary(keys::kTizenApplicationAppControlMimeKey,
      &mime_dict)) {
    mime_dict->GetString(
        keys::kTizenApplicationAppControlChildNameAttrKey, &mime);
  }

  std::string onreset(kEnabledValue);
  const parser::DictionaryValue* onreset_dict;
  if (control_dict.GetDictionary(keys::kTizenApplicationAppControlOnResetKey,
      &onreset_dict)) {
    onreset_dict->GetString(
        keys::kTizenApplicationAppControlChildNameAttrKey, &onreset);
  }

  aplist->controls.emplace_back(src, operation, uri, mime, onreset);
}

}  // namespace

AppControlHandler::AppControlHandler() {
}

AppControlHandler::~AppControlHandler() {
}

bool AppControlHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<AppControlInfoList> aplist(new AppControlInfoList());
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kTizenApplicationAppControlsKey, &value))
    return true;

  if (value->GetType() == parser::Value::TYPE_LIST) {
    // multiple entries
    const parser::ListValue* list;
    value->GetAsList(&list);
    for (const auto& item : *list) {
      const parser::DictionaryValue* control_dict;
      if (!item->GetAsDictionary(&control_dict)) {
        *error = "Parsing app-control element failed";
        return false;
      }

      ParseAppControlEntryAndStore(*control_dict, aplist.get());
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    // single entry
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);
    ParseAppControlEntryAndStore(*dict, aplist.get());
  } else {
    *error = "Cannot parse app-control element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(aplist);
  return true;
}

bool AppControlHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const AppControlInfoList& app_controls =
       static_cast<const AppControlInfoList&>(data);

  for (const auto& item : app_controls.controls) {
    if (item.src().empty()) {
      *error = "The src child element of app-control element is obligatory";
      return false;
    }

    const std::string& operation = item.operation();
    if (operation.empty()) {
      *error =
          "The operation child element of app-control element is obligatory";
      return false;
    }

    if (!parser::utils::IsValidIRI(operation)) {
      *error =
          "The operation child element of app-control element is not valid url";
      return false;
    }

    const std::string& onreset = item.onreset();
    if (onreset != kEnabledValue && onreset != kDisabledValue) {
      *error =
          "The improper value was given for appcontrol on-reset";
      return false;
    }
  }
  return true;
}

std::string AppControlHandler::Key() const {
  return keys::kTizenApplicationAppControlsKey;
}

AppControlInfo::AppControlInfo(
    const std::string& src,
    const std::string& operation,
    const std::string& uri,
    const std::string& mime,
    const std::string& onreset)
    : src_(src),
      operation_(operation),
      uri_(uri),
      mime_(mime),
      onreset_(onreset) {}

}   // namespace parse
}   // namespace wgt
