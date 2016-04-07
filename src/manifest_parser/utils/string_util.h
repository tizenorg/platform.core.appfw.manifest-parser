// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_PARSER_UTILS_STRING_UTIL_H_
#define MANIFEST_PARSER_UTILS_STRING_UTIL_H_

#include <string>

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace parser {
namespace utils {

// utf8 representation of unicode BIDI characters
LIBSCL_EXPORT_API extern const char kRightToLeftMark[];
LIBSCL_EXPORT_API extern const char kLeftToRightMark[];
LIBSCL_EXPORT_API extern const char kLeftToRightEmbeddingMark[];
LIBSCL_EXPORT_API extern const char kRightToLeftEmbeddingMark[];
LIBSCL_EXPORT_API extern const char kPopDirectionalFormatting[];
LIBSCL_EXPORT_API extern const char kLeftToRightOverride[];
LIBSCL_EXPORT_API extern const char kRightToLeftOverride[];

/**
 * @brief CollapseWhitespaceUTF8
 *        Replaces all utf-8 whitespace characters sequences with single space
 *        character. Whitespace at the beginning and at the end are removed
 *        completely.
 *
 * @param text input string
 * @return collapsed string
 */
LIBSCL_EXPORT_API std::string CollapseWhitespaceUTF8(const std::string& text);

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
LIBSCL_EXPORT_API std::string StripWrappingBidiControlCharactersUTF8(const std::string& text);

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
LIBSCL_EXPORT_API std::string GetDirTextUTF8(const std::string& text, const std::string& dir);

/**
 * @brief GetDirUTF8Start
 *        Returns bidi format character.
 *
 * @param dir chosen bidi format defined as "dir" attribute in w3c spec,
 *        allowed values: "ltr", "rtl", "lro", "rlo"
 * @return utf-8 character
 */
LIBSCL_EXPORT_API std::string GetDirUTF8Start(const std::string& dir);

/**
 * @brief GetDirUTF8End
 *        Return PDF (Pop Directional Format) character encoded in utf-8
 *
 * @return UTF-8 PDF bidi character
 */
LIBSCL_EXPORT_API std::string GetDirUTF8End();

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
LIBSCL_EXPORT_API std::string DecodePercentEscapedCharacter(const std::string& text);

}  // namespace utils
}  // namespace parser

#endif  // MANIFEST_PARSER_UTILS_STRING_UTIL_H_
