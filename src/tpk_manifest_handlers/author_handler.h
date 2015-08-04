// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_AUTHOR_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_AUTHOR_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class AuthorInfo : public parser::ManifestData {
 public:
  AuthorInfo(const std::string& email,
             const std::string& href,
             const std::string& name)
             : email_(email),
               href_(href),
               name_(name) {}
  ~AuthorInfo() override {}

  const std::string& email() const {
    return email_;
  }
  const std::string& href() const {
    return href_;
  }
  const std::string& name() const {
    return name_;
  }

 private:
  std::string email_;
  std::string href_;
  std::string name_;
};

struct AuthorInfoList : public parser::ManifestData {
  std::vector<AuthorInfo> items;
};

class AuthorHandler : public parser::ManifestHandler {
 public:
  AuthorHandler() {}
  ~AuthorHandler() override {}

  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  bool Validate(
      const parser::ManifestData& data,
      const parser::ManifestDataMap& handlers_output,
      std::string* error) const override;
  std::string Key() const override;
};

}   // namespace parse
}   // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_AUTHOR_HANDLER_H_
