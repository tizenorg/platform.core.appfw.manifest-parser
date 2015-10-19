// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef UTILS_STRING_UTIL_H_
#define UTILS_STRING_UTIL_H_

#include <string>

namespace parser {
namespace utils {

// utf8 representation of unicode BIDI characters
extern const char kRightToLeftMark[];
extern const char kLeftToRightMark[];
extern const char kLeftToRightEmbeddingMark[];
extern const char kRightToLeftEmbeddingMark[];
extern const char kPopDirectionalFormatting[];
extern const char kLeftToRightOverride[];
extern const char kRightToLeftOverride[];

/**
 * @brief CollapseWhitespaceUTF8
 *        Replaces all utf-8 whitespace characters sequences with single space
 *        character. Whitespace at the beginning and at the end are removed
 *        completely.
 *
 * @param text input string
 * @return collapsed string
 */
std::string CollapseWhitespaceUTF8(const std::string& text);

/**
 * @brief StripWrappingBidiControlCharactersUTF8
 *        Remove utf-8 bidi characters.
 *
 * Function removes single PDF bidi character at the end if exists.
 * Function removes one single LTR|RTL|LRO|RLO bidi character at the beginning
 * if exists.
 *
 * @param text input string
 * @return transformed string
 */
std::string StripWrappingBidiControlCharactersUTF8(const std::string& text);

/**
 * @brief GetDirTextUTF8
 *        Adds given bidi character to beginning of string and adds PDF bifi
 *        character
 *
 * @param text input string
 * @param dir chosen bidi format defined as "dir" attribute in w3c spec,
 *        allowed values: "ltr", "rtl", "lro", "rlo"
 * @return transformed string
 */
std::string GetDirTextUTF8(const std::string& text, const std::string& dir);

/**
 * @brief GetDirUTF8Start
 *        Returns bidi format character.
 *
 * @param dir chosen bidi format defined as "dir" attribute in w3c spec,
 *        allowed values: "ltr", "rtl", "lro", "rlo"
 * @return utf-8 character
 */
std::string GetDirUTF8Start(const std::string& dir);

/**
 * @brief GetDirUTF8End
 *        Return PDF (Pop Directional Format) character encoded in utf-8
 *
 * @return UTF-8 PDF bidi character
 */
std::string GetDirUTF8End();

/**
 * @brief DecodePercentEscapedCharacter
 *        Decodes url-encoded characters in string.
 *
 * This should be used to decoded path in configuration files where url spec
 * is allowed.
 *
 * @param path string with url-encoded characters
 * @return decoded value of text
 */
std::string DecodePercentEscapedCharacter(const std::string& text);

}  // namespace utils
}  // namespace parser

#endif  // UTILS_STRING_UTIL_H_
