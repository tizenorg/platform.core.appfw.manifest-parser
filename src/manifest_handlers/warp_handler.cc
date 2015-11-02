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

namespace {
const char kAccessKey[] = "widget.access";
const char kWidgetNamespacePrefix[] = "http://www.w3.org/ns/widgets";
const char kAccessSubdomainsKey[] = "@subdomains";
const char kAccessOriginKey[] = "@origin";
}

void WarpHandler::ParseSingleAccessElement(
    const parser::DictionaryValue& item_dict,
    std::shared_ptr<WarpInfo> info) {
  std::string domain_name;
  std::string subdomains_str;
  bool subdomains = false;

  if (item_dict.HasKey(kAccessSubdomainsKey)) {
    item_dict.GetString(kAccessSubdomainsKey, &subdomains_str);
    if (subdomains_str == "true")
      subdomains = true;
  }
  // TODO(w.kosowicz): address validation
  item_dict.GetString(kAccessOriginKey, &domain_name);
  if (domain_name == "*")
    subdomains = true;
  info->set_access_element(std::make_pair(domain_name, subdomains));
}

void WarpHandler::ParseAccessElements(
    const parser::Manifest& manifest,
    std::shared_ptr<WarpInfo> info) {
  if (!manifest.HasPath(kAccessKey))
    return;

  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  if (manifest.Get(kAccessKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (parser::VerifyElementNamespace(*dict, kWidgetNamespacePrefix))
        ParseSingleAccessElement(*dict, info);
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list) {
        if (item->GetAsDictionary(&dict)) {
          if (!parser::VerifyElementNamespace(*dict,
                                              kWidgetNamespacePrefix))
            continue;
          ParseSingleAccessElement(*dict, info);
        }
      }
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

std::string WarpInfo::key() {
  return kAccessKey;
}

std::string WarpHandler::Key() const {
  return kAccessKey;
}

}  // namespace parse
}  // namespace wgt
