// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/warp_handler.h"

#include <string.h>
#include <cassert>
#include <map>
#include <utility>

#include "manifest_handlers/application_manifest_constants.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

void WarpHandler::ParseSingleAccessElement(
    const parser::DictionaryValue& item_dict,
    std::shared_ptr<WarpInfo> info) {
  std::string domain_name;
  std::string subdomains_str;
  bool subdomains = false;

  if (item_dict.HasKey(keys::kAccessSubdomainsKey)) {
    item_dict.GetString(keys::kAccessSubdomainsKey, &subdomains_str);
    if (subdomains_str == "true")
      subdomains = true;
  }
  // TODO(w.kosowicz): address validation
  item_dict.GetString(keys::kAccessOriginKey, &domain_name);
  if (domain_name == "*")
    subdomains = true;
  info->set_access_element(std::make_pair(domain_name, subdomains));
}

void WarpHandler::ParseAccessElements(
    const parser::Manifest& manifest,
    std::shared_ptr<WarpInfo> info) {
  if (!manifest.HasPath(keys::kAccessKey))
    return;

  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  if (manifest.Get(keys::kAccessKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      ParseSingleAccessElement(*dict, info);
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          ParseSingleAccessElement(*dict, info);
    }
  }
}

bool WarpHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* /*error*/) {
  std::shared_ptr<WarpInfo> info(new WarpInfo);
  ParseAccessElements(manifest, info);
  *output = std::static_pointer_cast<parser::ManifestData>(info);
  return true;
}

std::string WarpHandler::Key() const {
  return keys::kAccessKey;
}

}  // namespace parse
}  // namespace wgt
