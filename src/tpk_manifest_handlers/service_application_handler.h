// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_SERVICE_APPLICATION_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_SERVICE_APPLICATION_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class ServiceApplicationInfo : public parser::ManifestData {
 public:
  ServiceApplicationInfo(const std::string& appid,
                         const std::string& auto_restart,
                         const std::string& exec,
                         const std::string& on_boot,
                         const std::string& type)
                         : appid_(appid),
                           auto_restart_(auto_restart),
                           exec_(exec),
                           on_boot_(on_boot),
                           type_(type) {}

  const std::string& appid() const {
    return appid_;
  }
  const std::string& auto_restart() const {
    return auto_restart_;
  }
  const std::string& exec() const {
    return exec_;
  }
  const std::string& on_boot() const {
    return on_boot_;
  }
  const std::string& type() const {
    return type_;
  }

 private:
  std::string appid_;
  std::string auto_restart_;
  std::string exec_;
  std::string on_boot_;
  std::string type_;
};

struct ServiceApplicationInfoList : public parser::ManifestData {
  std::vector<ServiceApplicationInfo> items;
};

class ServiceApplicationHandler : public parser::ManifestHandler {
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

#endif  // TPK_MANIFEST_HANDLERS_SERVICE_APPLICATION_HANDLER_H_
