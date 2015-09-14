// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/privileges_handler.h"

#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/values.h"
#include "utils/iri_util.h"
#include "utils/logging.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

// TODO(m.socha): Handler is very similar to wgt permissions_handler.
//                It should be refactored.

namespace tpk {
namespace parse {

namespace keys = tpk::application_keys;

bool PrivilegesHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<PrivilegesInfo> privileges_info(new PrivilegesInfo());
  parser::Value* privileges_value = nullptr;
  if (!manifest.Get(keys::kPrivilegesKey, &privileges_value))
    return true;
  parser::DictionaryValue* privileges_dict = nullptr;
  if (!privileges_value->GetAsDictionary(&privileges_dict)) {
    *error = "Failed to parse <privileges> tag";
    return false;
  }

  parser::Value* value = nullptr;
  if (!privileges_dict->Get(keys::kPrivilegeKey, &value))
    return true;

  std::unique_ptr<parser::ListValue> privileges_list;
  if (value->IsType(parser::Value::TYPE_DICTIONARY)) {
    privileges_list.reset(new parser::ListValue);
    privileges_list->Append(value->DeepCopy());
  } else {
    parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    if (list)
      privileges_list.reset(list->DeepCopy());
  }

  if (!privileges_list) {
    *error = "Invalid value of privileges.";
    return false;
  }
  for (parser::ListValue::const_iterator it = privileges_list->begin();
       it != privileges_list->end(); ++it) {
    parser::DictionaryValue* dictionary_value = nullptr;
    (*it)->GetAsDictionary(&dictionary_value);

    std::string privilege;
    if (!dictionary_value ||
        !dictionary_value->GetString(
            keys::kPrivilegeTextKey, &privilege) ||
        privilege.empty())
      continue;

    privileges_info->AddPrivilege(privilege);
  }

  *output = std::static_pointer_cast<parser::ManifestData>(privileges_info);
  return true;
}

std::string PrivilegesHandler::Key() const {
  return keys::kPrivilegesKey;
}

}   // namespace parse
}   // namespace tpk
