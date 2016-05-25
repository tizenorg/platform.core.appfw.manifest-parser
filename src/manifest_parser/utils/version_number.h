// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef MANIFEST_PARSER_UTILS_VERSION_NUMBER_H_
#define MANIFEST_PARSER_UTILS_VERSION_NUMBER_H_

#include <string>
#include <vector>

namespace utils {

/**
 * @brief The VersionNumber class
 *        Representation of version number
 *
 * This class should be used to store version number. Version number is allowed
 * to be constructed of number segments connected with single dot character.
 *
 * Every new version number should be validate be check result of member
 * function call @ref IsValid()
 */
class VersionNumber {
 public:
  explicit VersionNumber(const std::string& str);

  bool IsValid() const;
  int Compare(const VersionNumber& other) const;
  VersionNumber& Trim(unsigned size);

  bool operator<(const VersionNumber& other) const;
  bool operator>=(const VersionNumber& other) const;
  bool operator==(const VersionNumber& other) const;
  bool operator!=(const VersionNumber& other) const;
  bool operator>(const VersionNumber& other) const;
  bool operator<=(const VersionNumber& other) const;

  std::string ToString() const;

  /**
   * @brief IsValidTizenPackageVersion
   *        Checks if version is valid tizen package version number.
   *
   * Number should follow: a.b.c where
   *    0 <= a <= 255
   *    0 <= b <= 255
   *    0 <= c <= 65535
   *
   * b and c part are optional
   *
   * @return true if version is ok
   */
  bool IsValidTizenPackageVersion() const;

 private:
  std::vector<int> parts_;
};

}  // namespace utils

#endif  // MANIFEST_PARSER_UTILS_VERSION_NUMBER_H_
