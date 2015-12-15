// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "../manifest_parser/utils/language_tag_validator.h"

#include <boost/algorithm/string.hpp>
#include <vector>

#include "../manifest_parser/utils/logging.h"
#include "../manifest_parser/utils/w3c_languages.h"

namespace ba = boost::algorithm;

namespace {
const char kTagDelimiter[] = "-";
const int kSingletonTagSize = 1;
const int kMaximumExtensionTagSize = 8;

std::string Capitalized(const std::string& input) {
  std::string output = input;
  ba::to_lower(output);
  output[0] = toupper(output[0]);
  return output;
}

std::string ToLower(const std::string& input) {
  std::string output = input;
  ba::to_lower(output);
  return output;
}

std::string ToUpper(const std::string& input) {
  std::string output = input;
  ba::to_upper(output);
  return output;
}

}  // namespace

namespace utils {
namespace w3c_languages {

bool ValidateLanguageTag(const std::string& tag) {
  // algorithm based on http://www.w3.org/International/articles/language-tags/
  std::vector<std::string> splitted_tag;
  if (tag.empty()) {
    LOG(ERROR) << "tag is empty";
    return false;
  }
  boost::split(splitted_tag, tag, boost::is_any_of(kTagDelimiter));
  auto current_item = splitted_tag.begin();
  // main language validation
  if (current_item != splitted_tag.end() &&
      !lang_set::ValidateOnlyLanguage(*current_item)) {
    LOG(ERROR) << "Invalid main language tag given";
    return false;
  }
  ++current_item;
  if (current_item == splitted_tag.end()) {
    return true;
  }
  // extlang validation
  auto lang_pair = kW3CExtLang.find(ToLower(*current_item));
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
  if (kW3CScript.find(Capitalized(*current_item)) != kW3CScript.end()) {
    ++current_item;
    if (current_item == splitted_tag.end())
      return true;
  }
  // region subtag validation
  if (kW3CRegion.find(ToUpper(*current_item)) != kW3CRegion.end()) {
    ++current_item;
    if (current_item == splitted_tag.end())
      return true;
  }

  // TODO(w.kosowicz): variant prefix check
  if (kW3CVariant.find(ToLower(*current_item)) != kW3CVariant.end()) {
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


