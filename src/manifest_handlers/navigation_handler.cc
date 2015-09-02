// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/navigation_handler.h"

#include <boost/tokenizer.hpp>

#include "manifest_handlers/application_manifest_constants.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

namespace {

const boost::char_separator<char> navigation_separator(" ");

}  // namespace

AllowedNavigationInfo::AllowedNavigationInfo(const std::string&
                                             allowed_domains) {
  boost::tokenizer<boost::char_separator<char>> tokens(
      allowed_domains, navigation_separator);
  for (auto& item : tokens) {
    allowed_domains_.push_back(item);
  }
}

AllowedNavigationInfo::~AllowedNavigationInfo() {
}

NavigationHandler::NavigationHandler() {
}

NavigationHandler::~NavigationHandler() {
}

bool NavigationHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {

  const parser::Value* value = nullptr;
  if (!manifest.Get(keys::kAllowNavigationKey, &value))
    return true;
  const parser::DictionaryValue* dict = nullptr;
  if (!value->GetAsDictionary(&dict)) {
    const parser::ListValue* list = nullptr;
    if (value->GetAsList(&list)) {
      for (auto& item : *list) {
        const parser::DictionaryValue* candidate = nullptr;
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
  if (!VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix))
    return true;

  std::string allowed_domains;
  if (!dict->GetString(keys::kXmlTextKey, &allowed_domains)) {
    *error = "Invalid value of allow-navigation.";
    return false;
  }
  if (allowed_domains.empty()) {
    *output = std::static_pointer_cast<parser::ManifestData>(
        std::make_shared<AllowedNavigationInfo>(""));
    return true;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(
      std::make_shared<AllowedNavigationInfo>(allowed_domains));
  return true;
}

bool NavigationHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* /*error*/) const {
  const AllowedNavigationInfo& navi_info =
      static_cast<const AllowedNavigationInfo&>(data);
  // TODO(j.izydorczyk): There should be done 'navi_info' validity check.
  (void) navi_info;
  return true;
}

std::string NavigationHandler::Key() const {
  return keys::kAllowNavigationKey;
}

}  // namespace parse
}  // namespace wgt
