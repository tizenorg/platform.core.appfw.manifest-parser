// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/profile_handler.h"

#include <cassert>
#include <map>
#include <utility>

#include "manifest_parser/manifest_util.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

namespace keys = tpk::application_keys;

namespace {

const char kProfileNameKey[] = "@name";

}  // namespace

bool ProfileHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  if (!manifest.HasPath(keys::kProfileKey))
    return true;

  std::shared_ptr<ProfileInfo> profile_info(new ProfileInfo());
  for (auto& item : parser::GetOneOrMany(manifest.value(), keys::kProfileKey,
                                         "")) {
    std::string name;
    if (!item->GetString(kProfileNameKey, &name)) {
      *error = "Missing 'name' attribute in profile tag";
      return false;
    }
    profile_info->AddProfile(name);
  }

  *output = std::static_pointer_cast<parser::ManifestData>(profile_info);
  return true;
}

std::string ProfileHandler::Key() const {
  return keys::kProfileKey;
}

}  // namespace parse
}  // namespace tpk
