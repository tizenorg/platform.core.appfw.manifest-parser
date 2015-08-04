// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_AUTHOR_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_AUTHOR_HANDLER_H_

#include <memory>
#include <string>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class AuthorInfo : public parser::ManifestData {
 public:
  void set_email(const std::string& email) {
    email_ = email;
  }
  void set_href(const std::string& href) {
    href_ = href;
  }
  void set_name(const std::string& name) {
    name_ = name;
  }

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

class AuthorHandler : public parser::ManifestHandler {
 public:
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
