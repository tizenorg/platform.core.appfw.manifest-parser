// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "manifest_parser/utils/version_number.h"

#include <gtest/gtest.h>


namespace utils {

TEST(VersionNumber_Correct, Basic) {
  ASSERT_TRUE(VersionNumber("0").IsValid());
  ASSERT_TRUE(VersionNumber("1").IsValid());
  ASSERT_TRUE(VersionNumber("1.2").IsValid());
  ASSERT_TRUE(VersionNumber("1.2.3").IsValid());
  ASSERT_TRUE(VersionNumber("1.2.3.4").IsValid());
}

TEST(VersionNumber_Wrong, Basic) {
  ASSERT_FALSE(VersionNumber("0.").IsValid());
  ASSERT_FALSE(VersionNumber(".1").IsValid());
  ASSERT_FALSE(VersionNumber("1..2").IsValid());
  ASSERT_FALSE(VersionNumber("1.2.3.").IsValid());
  ASSERT_FALSE(VersionNumber("1.2..3.4").IsValid());
  ASSERT_FALSE(VersionNumber("1 2 3 4").IsValid());
  ASSERT_FALSE(VersionNumber("1.2.3beta").IsValid());
}

TEST(VersionNumber_GT, Basic) {
  ASSERT_GT(VersionNumber("2"), VersionNumber("1"));
  ASSERT_GT(VersionNumber("2.1"), VersionNumber("2"));
  ASSERT_GT(VersionNumber("2.2.3"), VersionNumber("2.2.1"));
  ASSERT_GT(VersionNumber("4"), VersionNumber("3.9.9.9.9"));
  ASSERT_GT(VersionNumber("4.4.2.1"), VersionNumber("4.3.2.1"));
}

TEST(VersionNumber_GE, Basic) {
  ASSERT_GE(VersionNumber("2"), VersionNumber("1"));
  ASSERT_GE(VersionNumber("2.1"), VersionNumber("2"));
  ASSERT_GE(VersionNumber("2.2.3"), VersionNumber("2.2.1"));
  ASSERT_GE(VersionNumber("4"), VersionNumber("3.9.9.9.9"));
  ASSERT_GE(VersionNumber("4.4.2.1"), VersionNumber("4.3.2.1"));
  ASSERT_GE(VersionNumber("0"), VersionNumber("0"));
  ASSERT_GE(VersionNumber("1"), VersionNumber("1"));
  ASSERT_GE(VersionNumber("2.1"), VersionNumber("2.1"));
  ASSERT_GE(VersionNumber("3.2.1"), VersionNumber("3.2.1"));
  ASSERT_GE(VersionNumber("4.3.2.1"), VersionNumber("4.3.2.1"));
}

TEST(VersionNumber_EQ, Basic) {
  ASSERT_EQ(VersionNumber("0"), VersionNumber("0"));
  ASSERT_EQ(VersionNumber("1"), VersionNumber("1"));
  ASSERT_EQ(VersionNumber("2.1"), VersionNumber("2.1"));
  ASSERT_EQ(VersionNumber("3.2.1"), VersionNumber("3.2.1"));
  ASSERT_EQ(VersionNumber("4.3.2.1"), VersionNumber("4.3.2.1"));
}

TEST(VersionNumber_NE, Basic) {
  ASSERT_NE(VersionNumber("1"), VersionNumber("0"));
  ASSERT_NE(VersionNumber("2.1"), VersionNumber("1.2"));
  ASSERT_NE(VersionNumber("3.2.1"), VersionNumber("3.2.2"));
  ASSERT_NE(VersionNumber("4.3.2.1"), VersionNumber("4.3.2"));
  ASSERT_NE(VersionNumber("4.3.2.1"), VersionNumber("4.3"));
  ASSERT_NE(VersionNumber("4"), VersionNumber("4.3"));
}

TEST(VersionNumber_LT, Basic) {
  ASSERT_LT(VersionNumber("1"), VersionNumber("2"));
  ASSERT_LT(VersionNumber("2"), VersionNumber("2.1"));
  ASSERT_LT(VersionNumber("2.2.1"), VersionNumber("2.2.3"));
  ASSERT_LT(VersionNumber("3.9.9.9.9"), VersionNumber("4"));
  ASSERT_LT(VersionNumber("4.3.2.1"), VersionNumber("4.4.2.1"));
}

TEST(VersionNumber_LE, Basic) {
  ASSERT_LE(VersionNumber("1"), VersionNumber("2"));
  ASSERT_LE(VersionNumber("2"), VersionNumber("2.1"));
  ASSERT_LE(VersionNumber("2.2.1"), VersionNumber("2.2.3"));
  ASSERT_LE(VersionNumber("3.9.9.9.9"), VersionNumber("4"));
  ASSERT_LE(VersionNumber("4.3.2.1"), VersionNumber("4.4.2.1"));
  ASSERT_LE(VersionNumber("0"), VersionNumber("0"));
  ASSERT_LE(VersionNumber("1"), VersionNumber("1"));
  ASSERT_LE(VersionNumber("2.1"), VersionNumber("2.1"));
  ASSERT_LE(VersionNumber("3.2.1"), VersionNumber("3.2.1"));
  ASSERT_LE(VersionNumber("4.3.2.1"), VersionNumber("4.3.2.1"));
}

TEST(VersionNumber_ToString, Basic) {
  ASSERT_EQ(VersionNumber("1").ToString(), "1");
  ASSERT_EQ(VersionNumber("0.3.2").ToString(), "0.3.2");
  ASSERT_EQ(VersionNumber("7.7.7.7.7.7.7").ToString(), "7.7.7.7.7.7.7");
}

TEST(VersionNumber_NegativeComponent, Basic) {
  ASSERT_FALSE(VersionNumber("-1").IsValid());
  ASSERT_FALSE(VersionNumber("1.-1.3").IsValid());
  ASSERT_FALSE(VersionNumber("1.1.-3").IsValid());
}

TEST(VersionNumber_IsValidTizenPackageVersion, Basic) {
  ASSERT_TRUE(VersionNumber("1").IsValidTizenPackageVersion());
  ASSERT_TRUE(VersionNumber("1.2").IsValidTizenPackageVersion());
  ASSERT_TRUE(VersionNumber("1.2.3").IsValidTizenPackageVersion());
  ASSERT_FALSE(VersionNumber("1.2.3.4").IsValidTizenPackageVersion());
  ASSERT_FALSE(VersionNumber("256.1").IsValidTizenPackageVersion());
  ASSERT_FALSE(VersionNumber("32.256.1").IsValidTizenPackageVersion());
  ASSERT_FALSE(VersionNumber("2.1.100000").IsValidTizenPackageVersion());
  ASSERT_TRUE(VersionNumber("255.255.65535").IsValidTizenPackageVersion());
}

}  // namespace utils
