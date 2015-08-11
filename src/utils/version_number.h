// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef UTILS_VERSION_NUMBER_H_
#define UTILS_VERSION_NUMBER_H_

#include <string>
#include <vector>

namespace utils {

class VersionNumber {
 public:
  explicit VersionNumber(const std::string& str);

  bool IsValid() const;
  int Compare(const VersionNumber& other) const;

  bool operator<(const VersionNumber& other) const;
  bool operator>=(const VersionNumber& other) const;
  bool operator==(const VersionNumber& other) const;
  bool operator!=(const VersionNumber& other) const;
  bool operator>(const VersionNumber& other) const;
  bool operator<=(const VersionNumber& other) const;

 private:
  std::vector<int> parts_;
};

}  // namespace utils

#endif  // UTILS_VERSION_NUMBER_H_
