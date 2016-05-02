// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "manifest_parser/utils/logging.h"

namespace utils {

log_priority LogLevelToPriority(LogLevel level) {
  switch (level) {
    case LogLevel::LOG_ERROR:
      return log_priority::DLOG_ERROR;
    case LogLevel::LOG_WARNING:
      return log_priority::DLOG_WARN;
    case LogLevel::LOG_INFO:
      return log_priority::DLOG_INFO;
    case LogLevel::LOG_DEBUG:
      return log_priority::DLOG_DEBUG;
    default:
      return log_priority::DLOG_UNKNOWN;
  }
}

}  // namespace utilschce
