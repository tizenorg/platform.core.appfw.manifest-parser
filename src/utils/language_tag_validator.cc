// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "utils/language_tag_validator.h"

#include <boost/algorithm/string.hpp>
#include <vector>
#include "utils/logging.h"
#include "utils/w3c_languages.h"

namespace {
const char kTagDelimiter[] = "-";
const int kSingletonTagSize = 1;
const int kMaximumExtensionTagSize = 8;
}  // namespace

namespace utils {
namespace w3c_languages {

bool ValidateLanguageTag(const std::string& tag) {
  std::vector<std::string> splitted_tag;
  if (tag.empty()) {
    LOG(ERROR) << "tag is empty";
    return false;
  }
  boost::split(splitted_tag, tag, boost::is_any_of(kTagDelimiter));
  auto current_item = splitted_tag.begin();
  // main language validation
  if (current_item != splitted_tag.end() &&
      kW3CLanguages.find(*current_item) == kW3CLanguages.end()) {
    LOG(ERROR) << "Invalid main language tag given";
    return false;
  }
  ++current_item;
  if (current_item == splitted_tag.end()) {
    return true;
  }
  // extlang validation
  auto lang_pair = kW3CExtLang.find(*current_item);
  if (lang_pair != kW3CExtLang.end()) {
    if (splitted_tag.front() == lang_pair->second) {
      ++current_item;
      if (current_item == splitted_tag.end())
        return true;
    } else {
      LOG(ERROR) << "Extlang does not match language";
      return false;
    }
  }
  // script subtag validation
  if (kW3CScript.find(*current_item) != kW3CScript.end()) {
    ++current_item;
    if (current_item == splitted_tag.end())
      return true;
  }
  // region subtag validation
  if (kW3CRegion.find(*current_item) != kW3CRegion.end()) {
    ++current_item;
    if (current_item == splitted_tag.end())
      return true;
  }

  // TODO(w.kosowicz): variant prefix check
  if (kW3CVariant.find(*current_item) != kW3CVariant.end()) {
    ++current_item;
    if (current_item == splitted_tag.end())
      return true;
  }
  // extension private tag validation
  if ((*current_item).size() != kSingletonTagSize) {
    LOG(ERROR) << "Singletion subtag should be of size " << kSingletonTagSize;
    return false;
  }
  ++current_item;
  if (current_item == splitted_tag.end())
    return false;
  for (auto it = current_item; it != splitted_tag.end(); ++current_item) {
    auto tag_length = (*current_item).size();
    if (tag_length > kMaximumExtensionTagSize) {
      LOG(ERROR) << "Any extensions should be maximum "
                 << kMaximumExtensionTagSize << "characters";
      return false;
    }
  }
  return true;
}

}  // namespace w3c_languages
}  // namespace utils


