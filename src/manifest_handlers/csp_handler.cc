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

  if (!VerifyElementNamespace(
        manifest, security_policy, keys::kTizenNamespacePrefix))
    return true;

  std::shared_ptr<CSPInfo> info(new CSPInfo);

  std::string security_rules;
  if (security_type_ == SecurityType::CSP)
    manifest.GetString(keys::kCSPText, &security_rules);
  else
    manifest.GetString(keys::kCSPTextReportOnly, &security_rules);
  info->set_security_rules(security_rules);

  *output = std::static_pointer_cast<parser::ManifestData>(info);
  return true;
}

std::string CSPHandler::Key() const {
  return security_type_ == SecurityType::CSP ?
        keys::kCSPKey : keys::kCSPTextReportOnly;
}

}  // namespace parse
}  // namespace wgt
