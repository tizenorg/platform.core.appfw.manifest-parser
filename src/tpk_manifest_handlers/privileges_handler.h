// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_PRIVILEGES_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_PRIVILEGES_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/permission_types.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace tpk {
namespace parse {

class LIBSCL_EXPORT_API PrivilegesInfo : public parser::ManifestData {
 public:
  /**
   * @brief key
   * @param key string
   */
  static std::string key();
  /**
   * @brief GetPrivileges
   * @return PrivilegesSet
   */
  const parser::PrivilegesSet& GetPrivileges() const {
    return privileges_;
  }
  /**
   * @brief AddPrivilege add privilege
   * @param privilege
   */
  void AddPrivilege(const std::string& privilege) {
    privileges_.insert(privilege);
  }

 private:
  parser::PrivilegesSet privileges_;
};

/**
 * @brief The PrivilegesHandler class
 *
 * Handler of tizen-manifest.xml for xml elements:
 *  <privileges>
 *  \_  <privilege>
 */
class LIBSCL_EXPORT_API PrivilegesHandler : public parser::ManifestHandler {
 public:
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  std::string Key() const override;
};

}   // namespace parse
}   // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_PRIVILEGES_HANDLER_H_
