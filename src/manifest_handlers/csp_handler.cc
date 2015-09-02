// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/csp_handler.h"

#include <string.h>
#include <cassert>
#include <map>
#include <utility>

#include "manifest_handlers/application_manifest_constants.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

bool CSPHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* /*error*/) {
  std::string security_policy = (security_type_ == SecurityType::CSP) ?
      keys::kCSPKey : keys::kCSPKeyReportOnly;
  const parser::Value* value = nullptr;
  if (!manifest.Get(security_policy, &value))
    return true;
  const parser::DictionaryValue* dict = nullptr;
  if (!value->GetAsDictionary(&dict)) {
    const parser::ListValue* list = nullptr;
    if (value->GetAsList(&list)) {
      const parser::DictionaryValue* candidate = nullptr;
      for (auto& item : *list) {
        if (item->GetAsDictionary(&candidate) &&
            parser::VerifyElementNamespace(
              *candidate, keys::kTizenNamespacePrefix)) {
          dict = candidate;
          break;
        }
      }
    }
  }
  if (!dict)
    return true;
  if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix))
    return true;

  std::shared_ptr<CSPInfo> info(new CSPInfo);

  std::string security_rules;
  dict->GetString(keys::kXmlTextKey, &security_rules);
  info->set_security_rules(security_rules);

  *output = std::static_pointer_cast<parser::ManifestData>(info);
  return true;
}

std::string CSPHandler::Key() const {
  return security_type_ == SecurityType::CSP ?
        keys::kCSPKey : keys::kCSPReportOnlyKey;
}

}  // namespace parse
}  // namespace wgt
