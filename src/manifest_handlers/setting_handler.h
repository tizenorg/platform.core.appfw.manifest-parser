// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_HANDLERS_SETTING_HANDLER_H_
#define MANIFEST_HANDLERS_SETTING_HANDLER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/values.h"
#include "manifest_parser/manifest_handler.h"

namespace wgt {
namespace parse {

class SettingInfo : public parser::ManifestData {
 public:
  enum class ScreenOrientation {
    PORTRAIT,
    LANDSCAPE,
    AUTO
  };

  enum class InstallLocation {
    INTERNAL,
    EXTERNAL,
    AUTO
  };

  enum class SoundMode {
    SHARED,
    EXCLUSIVE
  };

  SettingInfo();
  virtual ~SettingInfo();

  void set_hwkey_enabled(bool enabled) { hwkey_enabled_ = enabled; }
  bool hwkey_enabled() const { return hwkey_enabled_; }

  void set_screen_orientation(ScreenOrientation orientation) {
    screen_orientation_ = orientation;
  }

  ScreenOrientation screen_orientation() const { return screen_orientation_; }

  void set_encryption_enabled(bool enabled) { encryption_enabled_ = enabled; }
  bool encryption_enabled() const { return encryption_enabled_; }

  void set_context_menu_enabled(bool enabled) {
    context_menu_enabled_ = enabled;
  }
  bool context_menu_enabled() const { return context_menu_enabled_; }

  void set_background_support_enabled(bool enabled) {
    background_support_enabled_ = enabled;
  }
  bool background_support_enabled() const {
    return background_support_enabled_;
  }

  void set_install_location(InstallLocation installLocation) {
    install_location_ = installLocation;
  }
  InstallLocation install_location() const { return install_location_; }

  void set_no_display(bool enabled) { no_display_ = enabled; }
  bool no_display() const { return no_display_; }

  void set_indicator_presence(bool enabled) { indicator_presence_ = enabled; }
  bool indicator_presence() const { return indicator_presence_; }

  void set_backbutton_presence(bool enabled) { backbutton_presence_ = enabled; }
  bool backbutton_presence() const { return backbutton_presence_; }

  void set_user_agent(std::string user_agent) { user_agent_ = user_agent; }
  std::string user_agent() const { return user_agent_; }

  void set_sound_mode(SoundMode soundMode) { sound_mode_ = soundMode; }
  SoundMode sound_mode() const { return sound_mode_; }

  void set_background_vibration(bool enabled) {
    background_vibration_ = enabled;
  }
  bool background_vibration() const { return background_vibration_; }

 private:
  bool hwkey_enabled_;
  ScreenOrientation screen_orientation_;
  bool encryption_enabled_;
  bool context_menu_enabled_;
  bool background_support_enabled_;
  InstallLocation install_location_;
  bool no_display_;
  bool indicator_presence_;
  bool backbutton_presence_;
  std::string user_agent_;
  SoundMode sound_mode_;
  bool background_vibration_;
};

/**
 * @brief The SettingHandler class
 *
 * Handler of config.xml for xml elements:
 *  - <tizen:setting>.
 */
class SettingHandler : public parser::ManifestHandler {
 public:
  SettingHandler();
  virtual ~SettingHandler();

  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  bool Validate(
      const parser::ManifestData& application,
      const parser::ManifestDataMap& handlers_output,
      std::string* error) const override;
  std::string Key() const override;
};
}  // namespace parse
}  // namespace wgt

#endif  // MANIFEST_HANDLERS_SETTING_HANDLER_H_
