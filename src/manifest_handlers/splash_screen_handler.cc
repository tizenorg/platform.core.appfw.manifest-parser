// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/splash_screen_handler.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <cassert>
#include <map>
#include <utility>

#include "manifest_handlers/application_manifest_constants.h"

namespace bf = boost::filesystem;
namespace keys = wgt::application_widget_keys;
namespace wgt_parse = wgt::parse;

namespace {
const std::map<wgt_parse::Orientation, const char*> kOrientationMap = {
  {wgt_parse::Orientation::DEFAULT, keys::kSplashScreenDefault},
  {wgt_parse::Orientation::LANDSCAPE, keys::kSplashScreenLandscape},
  {wgt_parse::Orientation::PORTAIT, keys::kSplashScreenPortrait}
};
const char kTagDelimiter[] = " ";
const char kFirstPaint[] = "first-paint";
const char kUserInteractive[] = "user-interactive";
const char kComplete[] = "complete";
const char kCustom[] = "custom";
}  // namespace
namespace wgt {
namespace parse {

bool SplashScreenHandler::ParseSingleOrientation(
                                       const parser::Manifest& manifest,
                                       Orientation orientation,
                                       SplashScreenInfo* ss_info) {
  auto orientation_chosen = kOrientationMap.at(orientation);
  SplashScreenData splash_screen;
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  manifest.Get(orientation_chosen, &val);
  if (val->GetType() == parser::Value::Type::TYPE_LIST)
    return false;
  val->GetAsDictionary(&dict);
  if (!parser::VerifyElementNamespace(*dict, keys::kTizenNamespacePrefix))
    return false;
  splash_screen.orientation = orientation;
  if (!ParseColor(dict, &splash_screen) ||
      !ParseElement(dict, &splash_screen.background_image,
               keys::kSplashScreenBgImage) ||
      !ParseElement(dict, &splash_screen.image,
               keys::kSplashScreenImage) ||
      !ParseElement(dict, &splash_screen.image_border,
               keys::kSplashScreenImageBorder))
    return false;
  ss_info->set_splash_screen_data(splash_screen);
  return true;
}

bool SplashScreenHandler::ParseElement(const parser::DictionaryValue* dict,
                                       std::vector<std::string>* to_be_saved,
                                       const char* keyToParse) {
  // TODO(w.kosowicz) check which elements not declared should cause fail
  std::string element;
  dict->GetString(keyToParse, &element);
  boost::split(*to_be_saved, element, boost::is_any_of(kTagDelimiter));
  return true;
}

bool SplashScreenHandler::ParseColor(const parser::DictionaryValue* dict,
                                     SplashScreenData* splash_screen) {
  std::string background_color;
  dict->GetString(keys::kSplashScreenBgColor, &background_color);
  if (background_color.size() < 2 || background_color[0] != '#')
    return false;
  std::string only_hex = background_color.substr(1);
  for (auto single_char : only_hex) {
    if (!isxdigit(single_char))
      return false;
  }
  splash_screen->color = only_hex;
  return true;
}

bool SplashScreenHandler::ParseReadyWhen(const parser::Manifest& manifest,
                                         SplashScreenInfo* ss_info) {
  std::string ready_when;
  manifest.GetString(keys::kSplashScreenReadyWhen, &ready_when);
  if (ready_when == kFirstPaint)
    ss_info->set_ready_when(ReadyWhen::FIRSTPAINT);
  else if (ready_when == kUserInteractive)
    ss_info->set_ready_when(ReadyWhen::USERINTERACTIVE);
  else if (ready_when == kComplete)
    ss_info->set_ready_when(ReadyWhen::COMPLTE);
  else if (ready_when == kCustom)
    ss_info->set_ready_when(ReadyWhen::CUSTOM);
  else
    return false;
  return true;
}


bool SplashScreenHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<SplashScreenInfo> ss_info(new SplashScreenInfo);
  if (!ParseReadyWhen(manifest, ss_info.get()))
    return false;
  ParseSingleOrientation(manifest, Orientation::DEFAULT, ss_info.get());
  ParseSingleOrientation(manifest, Orientation::LANDSCAPE, ss_info.get());
  ParseSingleOrientation(manifest, Orientation::PORTAIT, ss_info.get());

  *output = std::static_pointer_cast<parser::ManifestData>(ss_info);
  return true;
}

bool SplashScreenHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {

  return true;
}

std::string SplashScreenHandler::Key() const {
  return keys::kSplashScreen;
}

}  // namespace parse
}  // namespace wgt
