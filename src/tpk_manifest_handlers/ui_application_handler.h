// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_UI_APPLICATION_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_UI_APPLICATION_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "tpk_manifest_handlers/ui_and_service_application_infos.h"

namespace tpk {
namespace parse {

class UIApplicationInfo : public parser::ManifestData {
 public:
  void set_appid(const std::string& appid) {
    appid_ = appid;
  }
  void set_exec(const std::string& exec) {
    exec_ = exec;
  }
  void set_launch_mode(const std::string& launch_mode) {
    launch_mode_ = launch_mode;
  }
  void set_multiple(const std::string& multiple) {
    multiple_ = multiple;
  }
  void set_nodisplay(const std::string& nodisplay) {
    nodisplay_ = nodisplay;
  }
  void set_taskmanage(const std::string& taskmanage) {
    taskmanage_ = taskmanage;
  }
  void set_type(const std::string& type) {
    type_ = type;
  }

  const std::string& appid() const {
    return appid_;
  }
  const std::string& exec() const {
    return exec_;
  }
  const std::string& launch_mode() const {
    return launch_mode_;
  }
  const std::string& multiple() const {
    return multiple_;
  }
  const std::string& nodisplay() const {
    return nodisplay_;
  }
  const std::string& taskmanage() const {
    return taskmanage_;
  }
  const std::string& type() const {
    return type_;
  }

 private:
  std::string appid_;
  std::string exec_;
  std::string launch_mode_;
  std::string multiple_;
  std::string nodisplay_;
  std::string taskmanage_;
  std::string type_;
};

struct UIApplicationSingleEntry : public parser::ManifestData {
  UIApplicationInfo ui_info;
  std::vector<AppControlInfo> app_control;
  std::vector<DataControlInfo> data_control;
  std::vector<MetaDataInfo> meta_data;
  ApplicationIconsInfo app_icons;
  std::vector<LabelInfo> label;
};

struct UIApplicationInfoList : public parser::ManifestData {
  std::vector<UIApplicationSingleEntry> items;
};

/**
 * @brief The UIApplicationHandler class
 *
 * Handler of tizen-manifest.xml for xml elements:
 *  <ui-application>
 *  \_  <label>
 *  \_  <icon>
 *  \_  <app-control>
 *  |   \_  <mime>
 *  |   \_  <operation>
 *  |   \_  <uri>
 *  \_  <metadata>
 *  \_  <datacontrol>
 */
class UIApplicationHandler : public parser::ManifestHandler {
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
  std::vector<std::string> PrerequisiteKeys() const override;
};

}   // namespace parse
}   // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_UI_APPLICATION_HANDLER_H_
