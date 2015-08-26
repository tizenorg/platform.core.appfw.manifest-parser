// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/platform_version.h"

namespace parser {

utils::VersionNumber GetCurrentPlatformVersion() {
  return utils::VersionNumber(TIZEN_VERSION);
}

}  // namespace parser
