// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/setting_handler.h"

#include <string.h>

#include <cassert>
#include <map>
#include <utility>

#include "manifest_handlers/application_manifest_constants.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

SettingInfo::SettingInfo()
    : hwkey_enabled_(true),
      screen_orientation_(ScreenOrientation::AUTO),
      encryption_enabled_(false),
      context_menu_enabled_(true),
      background_support_enabled_(false),
      install_location_(InstallLocation::AUTO),
      no_display_(false),
      indicator_presence_(true),
      backbutton_presence_(false),
      sound_mode_(SoundMode::SHARED),
      background_vibration_(false) {}

SettingInfo::~SettingInfo() {}

SettingHandler::SettingHandler() {}

SettingHandler::~SettingHandler() {}

bool SettingHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* /*error*/) {
  std::shared_ptr<SettingInfo> app_info(new SettingInfo);

  std::string hwkey;
  manifest.GetString(keys::kTizenHardwareKey, &hwkey);
  app_info->set_hwkey_enabled(hwkey != "disable");

  std::string screen_orientation;
  manifest.GetString(keys::kTizenScreenOrientationKey, &screen_orientation);
  if (strcasecmp("portrait", screen_orientation.c_str()) == 0)
    app_info->set_screen_orientation(SettingInfo::ScreenOrientation::PORTRAIT);
  else if (strcasecmp("landscape", screen_orientation.c_str()) == 0)
    app_info->set_screen_orientation(SettingInfo::ScreenOrientation::LANDSCAPE);
  else
    app_info->set_screen_orientation(SettingInfo::ScreenOrientation::AUTO);

  std::string encryption;
  manifest.GetString(keys::kTizenEncryptionKey, &encryption);
  app_info->set_encryption_enabled(encryption == "enable");

  std::string context_menu;
  manifest.GetString(keys::kTizenContextMenuKey, &context_menu);
  app_info->set_context_menu_enabled(context_menu != "disable");

  std::string background_support;
  manifest.GetString(keys::kTizenBackgroundSupportKey, &background_support);
  app_info->set_background_support_enabled(background_support == "enable");

  std::string install_location;
  manifest.GetString(keys::kTizenInstallLocationKey, &install_location);
  if (strcasecmp("internal-only", install_location.c_str()) == 0)
    app_info->set_install_location(SettingInfo::InstallLocation::INTERNAL);
  else if (strcasecmp("prefer-external", install_location.c_str()) == 0)
    app_info->set_install_location(SettingInfo::InstallLocation::EXTERNAL);

  std::string no_display;
  manifest.GetString(keys::kTizenNoDisplayKey, &no_display);
  app_info->set_no_display(no_display == "true");

  if (manifest.HasKey(keys::kTizenIndicatorPresenceKey)) {
    bool indicator_presence;
    manifest.GetBoolean(keys::kTizenIndicatorPresenceKey, &indicator_presence);
    app_info->set_indicator_presence(indicator_presence);
  }

  if (manifest.HasKey(keys::kTizenBackbuttonPresenceKey)) {
    bool backbutton_presence;
    manifest.GetBoolean(keys::kTizenBackbuttonPresenceKey,
                        &backbutton_presence);
    app_info->set_backbutton_presence(backbutton_presence);
  }

  if (manifest.HasKey(keys::kTizenUserAgentKey)) {
    std::string user_agent;
    manifest.GetString(keys::kTizenUserAgentKey, &user_agent);
    app_info->set_user_agent(user_agent);
  }

  std::string background_vibration;
  manifest.GetString(keys::kTizenBackgroundVibrationKey, &background_vibration);
  app_info->set_background_vibration(background_vibration == "enable");

  std::string sound_mode;
  manifest.GetString(keys::kTizenSoundModeKey, &sound_mode);
  if (strcasecmp("exclusive", sound_mode.c_str()) == 0)
    app_info->set_sound_mode(SettingInfo::SoundMode::EXCLUSIVE);

  *output = std::static_pointer_cast<parser::ManifestData>(app_info);
  return true;
}

bool SettingHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const SettingInfo& setting_info =
      static_cast<const SettingInfo&>(data);
  if (setting_info.screen_orientation() !=
          SettingInfo::ScreenOrientation::AUTO &&
      setting_info.screen_orientation() !=
          SettingInfo::ScreenOrientation::PORTRAIT &&
      setting_info.screen_orientation() !=
          SettingInfo::ScreenOrientation::LANDSCAPE) {
    *error = "Wrong value of screen orientation";
    return false;
  }

  if (setting_info.install_location() !=
          SettingInfo::InstallLocation::AUTO &&
      setting_info.install_location() !=
          SettingInfo::InstallLocation::INTERNAL &&
      setting_info.install_location() !=
          SettingInfo::InstallLocation::EXTERNAL) {
    *error = "Wrong value of install location";
    return false;
  }

  if (setting_info.sound_mode() !=
          SettingInfo::SoundMode::SHARED &&
      setting_info.sound_mode() !=
          SettingInfo::SoundMode::EXCLUSIVE) {
    *error = "Wrong value of screen sound mode";
    return false;
  }

  return true;
}

std::string SettingHandler::Key() const {
  return keys::kTizenSettingKey;
}

}  // namespace parse
}  // namespace wgt
