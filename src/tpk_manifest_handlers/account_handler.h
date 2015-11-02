// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_ACCOUNT_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_ACCOUNT_HANDLER_H_

#include <utility>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"

namespace tpk {
namespace parse {

struct SingleAccountInfo {
  bool multiple_account_support;
  std::string app_id;
  std::string provider_id;
  std::vector<std::pair<std::string, std::string>> icon_paths;
  std::vector<std::string> capabilities;
  std::vector<std::pair<std::string, std::string>> labels;
};

class AccountInfo : public parser::ManifestData {
 public:
  /**
   * @brief key
   * @param key string
   */
  static std::string key();

  const std::vector<SingleAccountInfo>& accounts() const {
    return accounts_;
  }

  void set_account(const SingleAccountInfo& single_account) {
    accounts_.push_back(single_account);
  }

 private:
  std::vector<SingleAccountInfo> accounts_;
};

/**
 * @brief The AccountHandler class
 *
 * Handler of tizen-manifest.xml for xml elements:
 *  <account>
 *  \_  <account-provider>
 *      \_  <icon>
 *      \_  <label>
 *      \_  <capability>
 */
class AccountHandler : public parser::ManifestHandler {
 public:
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
      std::string Key() const override;

 private:
      /**
   * @brief ParseSingleAccountElement parses single account element
   * @param item_dict
   * @param info
   * @param error
   * @return true everything okay
   */
  bool ParseSingleAccountElement(const parser::DictionaryValue* item_dict,
                                 std::shared_ptr<AccountInfo> info,
                                 std::string* error);
  /**
   * @brief ParseAccountIcons parse account icons
   * @param item_dict
   * @param singel_account
   * @return true if everything is okay
   */
  bool ParseAccountIcons(const parser::DictionaryValue* item_dict,
                         SingleAccountInfo* singel_account);
  /**
   * @brief ParseAccountProvider parses account provider
   * @param item_dict
   * @param info
   * @param error
   * @return if everything is fine
   */
  bool ParseAccountProvider(const parser::DictionaryValue* item_dict,
                             std::shared_ptr<AccountInfo> info,
                             std::string* error);
  /**
   * @brief ParseSingleAccountIcon parses single account icon
   * @param item_dict
   * @param singel_account
   * @return
   */
  bool ParseSingleAccountIcon(const parser::DictionaryValue* item_dict,
                              SingleAccountInfo* singel_account);
  /**
   * @brief ParseLabels parses single account label
   * @param item_dict
   * @param singel_account
   * @return true if everything is fine
   */
  bool ParseLabels(const parser::DictionaryValue* item_dict,
                         SingleAccountInfo* singel_account);
  /**
   * @brief ParseCapabilities pa
   * @param item_dict
   * @param singel_account
   * @return true if everything is fine
   */
  bool ParseCapabilities(const parser::DictionaryValue* item_dict,
                         SingleAccountInfo* singel_account);
};

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_ACCOUNT_HANDLER_H_
