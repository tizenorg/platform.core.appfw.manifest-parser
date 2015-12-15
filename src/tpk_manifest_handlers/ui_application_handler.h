// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_UI_APPLICATION_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_UI_APPLICATION_HANDLER_H_

#include <memory>
#include <string>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "tpk_manifest_handlers/common/application_handler.h"

namespace tpk {
namespace parse {

class UIApplicationInfo : public ApplicationInfo {
 public:
  UIApplicationInfo();
  /**
   * @brief key
   * @param key string
   */
  static std::string key();
  /**
   * @brief set_launch_mode sets launch mdoe
   * @param launch_mode
   */
  void set_launch_mode(const std::string& launch_mode) {
    launch_mode_ = launch_mode;
  }
  /**
   * @brief set_multiple sets multiple
   * @param multiple
   */
  void set_multiple(const std::string& multiple) {
    multiple_ = multiple;
  }
  /**
   * @brief set_nodisplay sets no display
   * @param nodisplay
   */
  void set_nodisplay(const std::string& nodisplay) {
    nodisplay_ = nodisplay;
  }
  /**
   * @brief set_taskmanage set task manage
   * @param taskmanage
   */
  void set_taskmanage(const std::string& taskmanage) {
    taskmanage_ = taskmanage;
  }
  void set_uigadget(const std::string& uigadget) {
    uigadget_ = uigadget;
  }
  void set_submode(const std::string& submode) {
    submode_ = submode;
  }
  void set_submode_mainid(const std::string& submode_mainid) {
    submode_mainid_ = submode_mainid;
  }
  void set_indicator_display(const std::string& indicator_display) {
    indicator_display_ = indicator_display;
  }
  void set_portrait_image(const std::string& portrait_image) {
    portrait_image_ = portrait_image;
  }
  void set_landscape_image(const std::string& landscape_image) {
    landscape_image_ = landscape_image;
  }
  void set_effectimage_type(const std::string& effectimage_type) {
    effectimage_type_ = effectimage_type;
  }
  void set_hwacceleration(const std::string& hwacceleration) {
    hwacceleration_ = hwacceleration;
  }

  /**
   * @brief launch_mode
   * @return launch mode string
   */
  const std::string& launch_mode() const {
    return launch_mode_;
  }
  /**
   * @brief multiple
   * @return multiple string
   */
  const std::string& multiple() const {
    return multiple_;
  }
  /**
   * @brief nodisplay
   * @return nodisplay string
   */
  const std::string& nodisplay() const {
    return nodisplay_;
  }
  /**
   * @brief taskmanage
   * @return taskmanage
   */
  const std::string& taskmanage() const {
    return taskmanage_;
  }

  const std::string& uigadget() const {
    return uigadget_;
  }

  const std::string& submode() const {
    return submode_;
  }

  const std::string& submode_mainid() const {
    return submode_mainid_;
  }

  const std::string& indicator_display() const {
    return indicator_display_;
  }

  const std::string& portrait_image() const {
    return portrait_image_;
  }

  const std::string& landscape_image() const {
    return landscape_image_;
  }

  const std::string& effectimage_type() const {
    return effectimage_type_;
  }

  const std::string& hwacceleration() const {
    return hwacceleration_;
  }

 private:
  std::string launch_mode_;
  std::string multiple_;
  std::string nodisplay_;
  std::string taskmanage_;
  std::string uigadget_;
  std::string submode_;
  std::string submode_mainid_;
  std::string indicator_display_;
  std::string portrait_image_;
  std::string landscape_image_;
  std::string effectimage_type_;
  std::string hwacceleration_;
};

struct UIApplicationSingleEntry :
    public ApplicationSingleEntry<UIApplicationInfo> {
  ApplicationImagesInfo app_images;
};

typedef ApplicationInfoList<UIApplicationSingleEntry>
    UIApplicationInfoList;

/**
 * @brief The UIApplicationHandler class
 *
 * Handler of tizen-manifest.xml for xml elements:
 *  <ui-application>
 *  \_  <label>
 *  \_  <icon>
 *  \_  <image>
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

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_UI_APPLICATION_HANDLER_H_
