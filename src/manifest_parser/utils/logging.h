// Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef MANIFEST_PARSER_UTILS_LOGGING_H_
#define MANIFEST_PARSER_UTILS_LOGGING_H_

#include <dlog/dlog.h>

#ifndef PROJECT_TAG
#error "PROJECT_TAG must be specified"
#endif

#ifdef LOG
#undef LOG
#endif

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace utils {

enum class LogLevel {
  LOG_ERROR,
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG,
};

log_priority LogLevelToPriority(LogLevel level);

template<LogLevel> struct LogTag;
template<> struct LogTag<LogLevel::LOG_ERROR> {
  static constexpr const char* value = "\033[1;31m| ERROR   |\033[0m";
};
template<> struct LogTag<LogLevel::LOG_WARNING> {
  static constexpr const char* value = "\033[1;33m| WARNING |\033[0m";
};
template<> struct LogTag<LogLevel::LOG_INFO>  {
  static constexpr const char* value = "\033[1;32m| INFO    |\033[0m";
};
template<> struct LogTag<LogLevel::LOG_DEBUG> {
  static constexpr const char* value = "\033[0m| DEBUG   |\033[0m";
};

class LogCatcher {
 public:
  explicit LogCatcher(LogLevel level, const char* tag)
    : level_(level), tag_(tag) { }
  void operator&(const std::ostream& str) const {
    dlog_print(LogLevelToPriority(level_), tag_.c_str(),
               static_cast<const std::ostringstream*>(&str)->str().c_str());
    if (level_ == LogLevel::LOG_ERROR) {
      std::cerr << static_cast<const std::ostringstream*>(&str)->str()
        << std::endl;
    }
  }
 private:
  LogLevel level_;
  std::string tag_;
};

}  // namespace utils

inline static const constexpr char* __tag_for_logging() {
  return "";
}

inline static const constexpr char* __tag_for_project() {
  return PROJECT_TAG;
}

// To be defined in class namespace if user want different log tag for given
// scope
#define SCOPE_LOG_TAG(TAG)                                                     \
  inline static const constexpr char* __tag_for_logging() {                    \
    return #TAG;                                                               \
  }                                                                            \

// Simple logging macro of following usage:
//   LOG(LEVEL) << object_1 << object_2 << object_n;
//     where:
//       LEVEL = ERROR | WARNING | INFO | DEBUG
#define LOG(LEVEL)                                                             \
    ::utils::LogCatcher(                                                       \
      ::utils::LogLevel::LOG_ ## LEVEL, __tag_for_project())                   \
      & std::ostringstream()                                                   \
      << std::string(::utils::LogTag<::utils::LogLevel::LOG_ ## LEVEL>::value) \
      << " " << std::setw(20) << std::left << __tag_for_logging()              \
      << std::setw(0) << " : "                                                 \

#endif  // MANIFEST_PARSER_UTILS_LOGGING_H_
