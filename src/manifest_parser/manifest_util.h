// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_PARSER_MANIFEST_UTIL_H_
#define MANIFEST_PARSER_MANIFEST_UTIL_H_

#include <boost/filesystem/path.hpp>
#include <libxml2/libxml/tree.h>

#include <string>
#include <map>
#include <memory>

#include "manifest_parser/manifest.h"

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


// Utilities for manipulating the on-disk storage of applications.
namespace parser {

// Loads an application manifest from the specified directory. Returns nullptr
// on failure, with a description of the error in |error|.
LIBSCL_EXPORT_API std::shared_ptr<Manifest> LoadManifest(
    const std::string& file_path, std::string* error);
LIBSCL_EXPORT_API std::string GetNodeDir(xmlNode* node, const std::string& inherit_dir);
LIBSCL_EXPORT_API std::string GetNodeText(xmlNode* root, const std::string& inherit_dir);
LIBSCL_EXPORT_API bool IsPropSupportDir(xmlNode* root, xmlAttr* prop);
LIBSCL_EXPORT_API bool IsElementSupportSpanAndDir(xmlNode* root);
LIBSCL_EXPORT_API bool IsTrimRequiredForElement(xmlNode* root);
LIBSCL_EXPORT_API bool IsTrimRequiredForProp(xmlNode* root, xmlAttr* prop);
LIBSCL_EXPORT_API std::unique_ptr<DictionaryValue> LoadXMLNode(
    xmlNode* root, const std::string& inherit_dir = "");

LIBSCL_EXPORT_API bool ValidateTizenApplicationId(const std::string& id);
LIBSCL_EXPORT_API bool ValidateTizenPackageId(const std::string& id);
LIBSCL_EXPORT_API bool ValidateTizenNativeId(const std::string& id);

LIBSCL_EXPORT_API bool ValidateEmailAddress(const std::string& email);

}  // namespace parser

#endif  // MANIFEST_PARSER_MANIFEST_UTIL_H_
