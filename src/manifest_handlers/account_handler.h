// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef MANIFEST_HANDLERS_ACCOUNT_HANDLER_H_
#define MANIFEST_HANDLERS_ACCOUNT_HANDLER_H_

#include <utility>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"

namespace wgt {
namespace parse {

struct SingleAccountInfo {
  bool multiple_account_support;
  std::vector<std::pair<std::string, std::string>> names;
  std::vector<std::pair<std::string, std::string>> icon_paths;
  std::vector<std::string> capabilities;
};

class AccountInfo : public parser::ManifestData {
 public:
  AccountInfo() {}
  ~AccountInfo() override {}

  const std::vector<SingleAccountInfo>& account() {
    return accounts_;
  }

  void set_account(const SingleAccountInfo& single_account) {
    accounts_.push_back(single_account);
  }

 private:
  std::vector<SingleAccountInfo> accounts_;
};

class AccountHandler : public parser::ManifestHandler {
 public:
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;

  std::string Key() const override;
 private:
  bool ParseSingleAccountElement(const parser::DictionaryValue* item_dict,
                                 std::shared_ptr<AccountInfo> info,
                                 std::string* error);
  bool ParseAccountIcons(const parser::DictionaryValue* item_dict,
                         SingleAccountInfo* singel_account);
  bool ParseSingleAccountIcon(const parser::DictionaryValue* item_dict,
                              SingleAccountInfo* singel_account);
  bool ParseAccountNames(const parser::DictionaryValue* item_dict,
                         SingleAccountInfo* singel_account);
  bool ParseSingleAccountName(const parser::DictionaryValue* item_dict,
                              SingleAccountInfo* singel_account);
  bool ParseCapabilities(const parser::DictionaryValue* item_dict,
                         SingleAccountInfo* singel_account);
};

}  // namespace parse
}  // namespace wgt

#endif  // MANIFEST_HANDLERS_ACCOUNT_HANDLER_H_
