// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.
#ifndef UTILS_LANGUAGE_TAG_VALIDATOR_H_
#define UTILS_LANGUAGE_TAG_VALIDATOR_H_

#include <string>
#include <unordered_set>
namespace utils {
namespace w3c_languages {
bool ValidateLanguageTag(const std::string& tag);
}  // namespace w3c_languages
}  // namespace utils
#endif  // UTILS_LANGUAGE_TAG_VALIDATOR_H_
