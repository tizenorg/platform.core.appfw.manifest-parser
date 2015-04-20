// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/platform_version.h"

#include <system_info.h>

namespace parser {
std::string GetCurrentPlatformVersion() {
  char* value = nullptr;
  // TODO(w.kosowicz) remove dependency from system-info capi
  if (system_info_get_platform_string("tizen.org/feature/platform.version",
      &value) == SYSTEM_INFO_ERROR_NONE) {
    std::string platformVersion(value);
    free(value);
    return platformVersion;
  }
  return std::string();
}
}  // namespace parser
