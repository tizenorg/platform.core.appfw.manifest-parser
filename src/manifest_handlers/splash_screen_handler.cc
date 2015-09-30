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
const std::map<wgt_parse::Orientation, const char*> orientation_map = {
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

SplashScreenInfo::SplashScreenInfo() {}
SplashScreenInfo::~SplashScreenInfo() {}

bool SplashScreenHandler::ParseSingleOrientation(
                                       const parser::Manifest& manifest,
                                       Orientation orientation,
                                       SplashScreenInfo* ss_info) {
  auto orientation_chosen = orientation_map.at(orientation);
  SplashScreenData splash_screen;
  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  manifest.Get(orientation_chosen, &val);
  val->GetAsDictionary(&dict);
  splash_screen.orientation = orientation;
  ParseColor(dict, &splash_screen);
  ParseElement(dict, &splash_screen.background_image,
               keys::kSplashScreenBgImage);
  ParseElement(dict, &splash_screen.image,
               keys::kSplashScreenImage);
  ParseElement(dict, &splash_screen.image_border,
               keys::kSplashScreenImageBorder);
  ss_info->set_splash_screen_data(splash_screen);

  return true;
}

bool SplashScreenHandler::ParseElement(const parser::DictionaryValue* dict,
                                       std::vector<std::string>* to_be_saved,
                                       const char* keyToParse) {
  std::string element;
  dict->GetString(keyToParse, &element);
  std::vector<std::string> element_vector;
  boost::split(element_vector, element, boost::is_any_of(kTagDelimiter));
  *to_be_saved = element_vector;
  return true;
}

bool SplashScreenHandler::ParseColor(const parser::DictionaryValue* dict,
                                     SplashScreenData* splash_screen) {
  std::string background_color;
  dict->GetString(keys::kSplashScreenBgColor, &background_color);
  if (background_color.size() != 7)
    return false;
  std::string only_hex = background_color.substr(1, background_color.size());
  if (only_hex.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
    return false;
  splash_screen->color = only_hex;
  return true;
}

bool SplashScreenHandler::ParseReadyWhen(const parser::Manifest& manifest,
                                         SplashScreenInfo* ss_info) {
  std::string ready_when;
  manifest.GetString(keys::kSplashScreenReadyWhen, &ready_when);
  if (ready_when == kFirstPaint)
    ss_info->set_ready_when(ReadyWhen::FirstPaint);
  else if (ready_when == kUserInteractive)
    ss_info->set_ready_when(ReadyWhen::UserInteractive);
  else if (ready_when == kComplete)
    ss_info->set_ready_when(ReadyWhen::Complete);
  else if (ready_when == kCustom)
    ss_info->set_ready_when(ReadyWhen::Custom);
  else
    return false;
  return true;
}


bool SplashScreenHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<SplashScreenInfo> ss_info(new SplashScreenInfo);
  ParseReadyWhen(manifest, ss_info.get());
  ParseSingleOrientation(manifest, Orientation::DEFAULT, ss_info.get());
  ParseSingleOrientation(manifest, Orientation::LANDSCAPE, ss_info.get());
  ParseSingleOrientation(manifest, Orientation::PORTAIT, ss_info.get());
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
