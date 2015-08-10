// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/app_control_handler.h"

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

void ParseAppControlAndStore(
    const parser::DictionaryValue& control_dict,
    AppControlInfoList* appcontrolinfo) {

  std::string operation;
  const parser::DictionaryValue* operation_dict;
  if (control_dict.GetDictionary(keys::kAppControlOperationKey,
                                 &operation_dict)) {
    operation_dict->GetString(
        keys::kAppControlNameChildKey, &operation);
  }

  std::string uri;
  const parser::DictionaryValue* uri_dict;
  if (control_dict.GetDictionary(keys::kAppControlURIKey,
                                 &uri_dict)) {
    uri_dict->GetString(
        keys::kAppControlNameChildKey, &uri);
  }

  std::string mime;
  const parser::DictionaryValue* mime_dict;
  if (control_dict.GetDictionary(keys::kAppControlMimeKey,
                                 &mime_dict)) {
    mime_dict->GetString(
        keys::kAppControlNameChildKey, &mime);
  }

  appcontrolinfo->items.emplace_back(operation, uri, mime);
}

}  // namespace

bool AppControlHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<AppControlInfoList> appcontrolinfo(new AppControlInfoList());
  parser::Value* value = nullptr;
  if (!manifest.Get(keys::kAppControlKey, &value))
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

      ParseAppControlAndStore(*control_dict, appcontrolinfo.get());
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    // single entry
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);
    ParseAppControlAndStore(*dict, appcontrolinfo.get());
  } else {
    *error = "Cannot parse app-control element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(appcontrolinfo);
  return true;
}

bool AppControlHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const AppControlInfoList& elements =
       static_cast<const AppControlInfoList&>(data);

  for (const auto& item : elements.items) {
    if (item.operation().empty()) {
      *error =
          "The operation child element of app-control element is obligatory";
      return false;
    }

    const std::string& uri = item.uri();
    if (uri.empty()) {
      *error =
          "The uri child element of app-control element is obligatory";
      return false;
    }

    if (!parser::utils::IsValidIRI(uri)) {
      *error =
          "The uri child element of app-control element is not valid url";
      return false;
    }

    const std::string& mime = item.mime();
    if (mime.empty()) {
      *error =
          "The mime child element of app-control element is obligatory";
      return false;
    }
  }
  return true;
}

std::string AppControlHandler::Key() const {
  return keys::kAppControlKey;
}

}   // namespace parse
}   // namespace tpk
