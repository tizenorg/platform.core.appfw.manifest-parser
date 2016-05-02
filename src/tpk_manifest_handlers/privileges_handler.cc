// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/privileges_handler.h"

#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/utils/iri_util.h"
#include "manifest_parser/utils/logger.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

namespace keys = tpk::application_keys;

namespace {
const char kPrivilegesKey[] = "manifest.privileges";
const char kPrivilegeKey[] = "privilege";
const char kPrivilegeTextKey[] = "#text";
}

bool PrivilegesHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<PrivilegesInfo> privileges_info(new PrivilegesInfo());
  parser::Value* privileges_value = nullptr;
  if (!manifest.Get(kPrivilegesKey, &privileges_value))
    return true;
  parser::DictionaryValue* privileges_dict = nullptr;
  if (!privileges_value->GetAsDictionary(&privileges_dict)) {
    *error = "Failed to parse <privileges> tag";
    return false;
  }
  for (auto& item : parser::GetOneOrMany(privileges_dict, kPrivilegeKey, "")) {
    std::string privilege;
    if (!item->GetString(kPrivilegeTextKey, &privilege) ||
        privilege.empty())
      continue;
    privileges_info->AddPrivilege(privilege);
  }
  *output = std::static_pointer_cast<parser::ManifestData>(privileges_info);
  return true;
}

std::string PrivilegesInfo::key() {
  return kPrivilegesKey;
}

std::string PrivilegesHandler::Key() const {
  return kPrivilegesKey;
}

}   // namespace parse
}   // namespace tpk
