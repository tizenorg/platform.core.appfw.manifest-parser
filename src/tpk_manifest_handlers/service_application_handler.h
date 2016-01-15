// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_SERVICE_APPLICATION_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_SERVICE_APPLICATION_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "tpk_manifest_handlers/common/application_handler.h"

namespace tpk {
namespace parse {

class ServiceApplicationInfo : public ApplicationInfo {
 public:
  /**
   * @brief key
   * @param key string
   */
  static std::string key();
  void set_type(const std::string& type) {
    type_ = type;
  }
  void set_process_pool(const std::string& process_pool) {
    process_pool_ = process_pool;
  }
  /**
   * @brief set_auto_restart sets auto restart
   * @param auto_restart
   */
  void set_auto_restart(const std::string& auto_restart) {
    auto_restart_ = auto_restart;
  }
  /**
   * @brief set_on_boot sets on boot
   * @param on_boot
   */
  void set_on_boot(const std::string& on_boot) {
    on_boot_ = on_boot;
  }

  const std::string& type() const {
    return type_;
  }
  const std::string& process_pool() const {
    return process_pool_;
  }
  /**
   * @brief auto_restart
   * @return auto_restart string
   */
  const std::string& auto_restart() const {
    return auto_restart_;
  }
  /**
   * @brief on_boot
   * @return on_boot string
   */
  const std::string& on_boot() const {
    return on_boot_;
  }

 private:
  std::string type_;
  std::string process_pool_;
  std::string auto_restart_;
  std::string on_boot_;
};

struct ServiceApplicationSingleEntry :
    public ApplicationSingleEntry<ServiceApplicationInfo> {
  std::vector<AppControlInfo> app_control;
  std::vector<BackgroundCategoryInfo> background_category;
  std::vector<DataControlInfo> data_control;
};

using ServiceApplicationInfoList =
    ApplicationInfoList<ServiceApplicationSingleEntry>;

/**
 * @brief The ServiceApplicationHandler class
 *
 * Handler of tizen-manifest.xml for xml elements:
 *  <service-application>
 *  \_  <label>
 *  \_  <icon>
 *  \_  <app-control>
 *  |   \_  <mime>
 *  |   \_  <operation>
 *  |   \_  <uri>
 *  \_  <metadata>
 *  \_  <datacontrol>
 */
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

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_SERVICE_APPLICATION_HANDLER_H_
