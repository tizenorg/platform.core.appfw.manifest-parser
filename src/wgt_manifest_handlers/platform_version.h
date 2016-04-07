// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef WGT_MANIFEST_HANDLERS_PLATFORM_VERSION_H_
#define WGT_MANIFEST_HANDLERS_PLATFORM_VERSION_H_

#include <string>

#include "manifest_parser/utils/version_number.h"

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace parser {
/**
 * @brief GetCurrentPlatformVersion
 * @return number version of the platform currently used
 */
LIBSCL_EXPORT_API utils::VersionNumber GetCurrentPlatformVersion();

/**
 * @brief GetMinimumPlatformVersion
 * @return number version of the minimum platform for current profile
 */
LIBSCL_EXPORT_API utils::VersionNumber GetMinimumPlatformVersion();

}  // namespace parser

#endif  // WGT_MANIFEST_HANDLERS_PLATFORM_VERSION_H_
