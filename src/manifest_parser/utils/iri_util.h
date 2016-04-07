// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef MANIFEST_PARSER_UTILS_IRI_UTIL_H_
#define MANIFEST_PARSER_UTILS_IRI_UTIL_H_

#include <string>

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace parser {
namespace utils {

/**
 * @brief IsValidIRI
 *        Validate IRI with libiri.
 *
 * Valid IRI is the one that have at least scheme part and any other component.
 *
 * @param iri_string iri string to check
 * @return bool if iri is valid
 */
LIBSCL_EXPORT_API bool IsValidIRI(const std::string& iri_string);

}  // namespace utils
}  // namespace parser

#endif  // MANIFEST_PARSER_UTILS_IRI_UTIL_H_
