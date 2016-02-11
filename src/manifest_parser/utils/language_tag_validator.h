// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.
#ifndef MANIFEST_PARSER_UTILS_LANGUAGE_TAG_VALIDATOR_H_
#define MANIFEST_PARSER_UTILS_LANGUAGE_TAG_VALIDATOR_H_

#include <string>
#include <unordered_set>

namespace utils {
namespace w3c_languages {

/**
 * @brief ValidateLanguageTag
 *        Checks validity of language tag.
 *
 * Languauge tag reference:
 *  - http://www.w3.org/International/articles/language-tags/
 *
 * @param language tag string
 * @param error output parameter for error
 * @return true if tag is correct
 */
bool ValidateLanguageTag(const std::string& tag, std::string* error = nullptr);

}  // namespace w3c_languages
}  // namespace utils
#endif  // MANIFEST_PARSER_UTILS_LANGUAGE_TAG_VALIDATOR_H_
