// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef MANIFEST_HANDLERS_PLATFORM_VERSION_H_
#define MANIFEST_HANDLERS_PLATFORM_VERSION_H_

#include <string>

#include "utils/version_number.h"

namespace parser {
/**
 * @brief GetCurrentPlatformVersion
 * @return number version of the platform currently used
 */
utils::VersionNumber GetCurrentPlatformVersion();

}  // namespace parser

#endif  // MANIFEST_HANDLERS_PLATFORM_VERSION_H_
