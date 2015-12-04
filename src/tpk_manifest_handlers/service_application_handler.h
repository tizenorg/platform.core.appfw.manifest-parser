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
#include "tpk_manifest_handlers/ui_and_service_application_infos.h"

namespace tpk {
namespace parse {

class ServiceApplicationInfo : public parser::ManifestData {
 public:
  /**
   * @brief key
   * @param key string
   */
  static std::string key();
  /**
   * @brief set_appid sets appid
   * @param appid
   */
  void set_appid(const std::string& appid) {
    appid_ = appid;
  }
  /**
   * @brief set_auto_restart sets auto restart
   * @param auto_restart
   */
  void set_auto_restart(const std::string& auto_restart) {
    auto_restart_ = auto_restart;
  }
  /**
   * @brief set_exec sets exec
   * @param exec
   */
  void set_exec(const std::string& exec) {
    exec_ = exec;
  }
  /**
   * @brief set_on_boot sets on boot
   * @param on_boot
   */
  void set_on_boot(const std::string& on_boot) {
    on_boot_ = on_boot;
  }
  /**
   * @brief set_type sets type
   * @param type
   */
  void set_type(const std::string& type) {
    type_ = type;
  }
  void set_process_pool(const std::string& process_pool) {
    process_pool_ = process_pool;
  }

  /**
   * @brief appid
   * @return appid string
   */
  const std::string& appid() const {
    return appid_;
  }
  /**
   * @brief auto_restart
   * @return auto_restart string
   */
  const std::string& auto_restart() const {
    return auto_restart_;
  }
  /**
   * @brief exec
   * @return exec string
   */
  const std::string& exec() const {
    return exec_;
  }
  /**
   * @brief on_boot
   * @return on_boot string
   */
  const std::string& on_boot() const {
    return on_boot_;
  }
  /**
   * @brief type
   * @return type string
   */
  const std::string& type() const {
    return type_;
  }
  const std::string& process_pool() const {
    return process_pool_;
  }

 private:
  std::string appid_;
  std::string auto_restart_;
  std::string exec_;
  std::string on_boot_;
  std::string type_;
  std::string process_pool_;
};

struct ServiceApplicationSingleEntry : public parser::ManifestData {
  ServiceApplicationInfo sa_info;
  std::vector<AppControlInfo> app_control;
  std::vector<DataControlInfo> data_control;
  std::vector<MetaDataInfo> meta_data;
  ApplicationIconsInfo app_icons;
  std::vector<LabelInfo> label;
};

struct ServiceApplicationInfoList : public parser::ManifestData {
  std::vector<ServiceApplicationSingleEntry> items;
};

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

}   // namespace parse
}   // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_SERVICE_APPLICATION_HANDLER_H_
