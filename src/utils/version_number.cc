// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "utils/version_number.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <algorithm>
#include <cstdlib>

namespace ba = boost::algorithm;

namespace utils {

VersionNumber::VersionNumber(const std::string& str) {
  std::string number = str;
  ba::trim(number);
  std::vector<std::string> tokens;
  ba::split(tokens, number, ba::is_any_of("."));
  for (auto& token : tokens) {
    char* end = nullptr;
    int value = strtol(token.c_str(), &end, 10);
    if (end != &*token.end() || token.empty()) {
      parts_.clear();
      return;
    }
    parts_.push_back(value);
  }
}

bool VersionNumber::IsValid() const {
  return !parts_.empty();
}

int VersionNumber::Compare(const VersionNumber& other) const {
  size_t i = 0;
  while (true) {
    if (i < parts_.size()) {
      if (i < other.parts_.size()) {
        if (parts_[i] > other.parts_[i]) {
          return 1;
        } else if (parts_[i] < other.parts_[i]) {
          return -1;
        }
      } else {
        return 1;
      }
    } else {
      if (i < other.parts_.size()) {
        return -1;
      } else {
        return 0;
      }
    }
    ++i;
  }
}

bool VersionNumber::operator<(const VersionNumber& other) const {
  return Compare(other) < 0;
}

bool VersionNumber::operator>=(const VersionNumber& other) const {
  return !this->operator<(other);
}

bool VersionNumber::operator==(const VersionNumber& other) const {
  return Compare(other) == 0;
}

bool VersionNumber::operator!=(const VersionNumber& other) const {
  return !this->operator==(other);
}

bool VersionNumber::operator>(const VersionNumber& other) const {
  return Compare(other) > 0;
}

bool VersionNumber::operator<=(const VersionNumber& other) const {
  return !this->operator>(other);
}

std::string VersionNumber::ToString() const {
  std::vector<std::string> strings;
  std::transform(parts_.begin(), parts_.end(), std::back_inserter(strings),
                 static_cast<std::string(*)(int)>(&std::to_string));
  return ba::join(strings, ".");
}

}  // namespace utils
