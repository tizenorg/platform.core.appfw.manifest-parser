// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/app_control_handler.h"

#include "manifest_parser/values.h"
#include "manifest_handlers/application_manifest_constants.h"
#include "utils/iri_util.h"
#include "utils/version_number.h"
#include "manifest_handlers/tizen_application_handler.h"

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

  std::string reload(kEnabledValue);
  control_dict.GetString(keys::kTizenApplicationAppControlReloadKey, &reload);

  aplist->controls.emplace_back(src, operation, uri, mime, reload);
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
    const parser::ManifestDataMap& handlers_output,
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

    const std::string& reload = item.reload();
    if (reload != kEnabledValue && reload != kDisabledValue) {
      *error = "The improper value was given for appcontrol reload";
      return false;
    }

    utils::VersionNumber supported_version("2.4");
    if (reload == kEnabledValue) {
      const TizenApplicationInfo& app_info =
          static_cast<const TizenApplicationInfo&>(
              *handlers_output.find(keys::kTizenApplicationKey)->second);
      utils::VersionNumber required_version(app_info.required_version());
      if (!required_version.IsValid()) {
        *error = "Cannot retrieve supported API version from widget";
        return false;
      }
      if (required_version < supported_version) {
        *error = "The reload attribute of app-control is applicable to"
            "platform >= 2.4";
        return false;
      }
    }
  }
  return true;
}

std::vector<std::string> AppControlHandler::PrerequisiteKeys() const {
  return {keys::kTizenApplicationKey};
}

std::string AppControlHandler::Key() const {
  return keys::kTizenApplicationAppControlsKey;
}

AppControlInfo::AppControlInfo(
    const std::string& src,
    const std::string& operation,
    const std::string& uri,
    const std::string& mime,
    const std::string& reload)
    : src_(src),
      operation_(operation),
      uri_(uri),
      mime_(mime),
      reload_(reload) {}

}   // namespace parse
}   // namespace wgt
