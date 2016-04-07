// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef MANIFEST_PARSER_UTILS_W3C_LANGUAGES_H_
#define MANIFEST_PARSER_UTILS_W3C_LANGUAGES_H_

#include <map>
#include <string>
#include <unordered_set>

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace utils {
namespace w3c_languages {
namespace lang_set {

/**
 * @brief ValidateOnlyLanguage
 *        Checks validity of language part of language tag.
 *
 * Languauge part is described:
 *  - http://www.w3.org/International/articles/language-tags/#language
 *
 * @param language language
 * @return true if language is correct
 */
LIBSCL_EXPORT_API bool ValidateOnlyLanguage(const std::string& language);

}  // namespace lang_set

LIBSCL_EXPORT_API extern const std::map<std::string, std::string> kW3CExtLang;
extern const std::unordered_set<std::string> kW3CScript;
extern const std::unordered_set<std::string> kW3CRegion;
extern const std::unordered_set<std::string> kW3CVariant;

}  // namespace w3c_languages
}  // namespace utils

#endif  // MANIFEST_PARSER_UTILS_W3C_LANGUAGES_H_
