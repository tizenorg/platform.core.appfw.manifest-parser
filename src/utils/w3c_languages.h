// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.
#ifndef UTILS_W3C_LANGUAGES_H_
#define UTILS_W3C_LANGUAGES_H_

#include <map>
#include <string>
#include <unordered_set>

namespace utils {
namespace w3c_languages {
extern const std::unordered_set<std::string> kW3CLanguages;
extern const std::map<std::string, std::string> kW3CExtLang;
extern const std::unordered_set<std::string> kW3CScript;
extern const std::unordered_set<std::string> kW3CRegion;
extern const std::unordered_set<std::string> kW3CVariant;


}  // namespace w3c_languages
}  // namespace utils

#endif  // UTILS_W3C_LANGUAGES_H_
