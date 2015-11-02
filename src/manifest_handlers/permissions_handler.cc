// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/permissions_handler.h"

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_parser/values.h"
#include "utils/logging.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

PermissionsInfo::PermissionsInfo() {
}

PermissionsInfo::~PermissionsInfo() {
}

PermissionsHandler::PermissionsHandler() {
}

PermissionsHandler::~PermissionsHandler() {
}

bool PermissionsHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  if (!manifest.HasPath(keys::kTizenPermissionsKey))
  {
    *error = "Invalid value of tizen permissions.";
    return false;
  }

  std::shared_ptr<PermissionsInfo> permissions_info(new PermissionsInfo);
  parser::PermissionSet api_permissions;

  for (const auto& dict : parser::GetOneOrMany(manifest.value(),
      keys::kTizenPermissionsKey, "")) {
    std::string permission;

    if (!dict->GetString(keys::kTizenPermissionsNameKey, &permission) ||
        permission.empty())
      continue;

    if (api_permissions.find(permission) != api_permissions.end())
      LOG(WARNING) << "Duplicated permission names found.";

    api_permissions.insert(permission);
  }

  permissions_info->SetAPIPermissions(api_permissions);

  *output = std::static_pointer_cast<parser::ManifestData>(permissions_info);
  return true;
}

bool PermissionsHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* /*error*/) const {
  const PermissionsInfo& perm_info = static_cast<const PermissionsInfo&>(data);
  // TODO(j.izydorczyk): there should be done 'perm_info' members validation
  (void) perm_info;
  return true;
}

std::string PermissionsHandler::Key() const {
  return keys::kTizenPermissionsKey;
}

}  // namespace parse
}  // namespace wgt
