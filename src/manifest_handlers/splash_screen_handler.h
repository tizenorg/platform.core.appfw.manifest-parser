// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_HANDLERS_SPLASH_SCREEN_HANDLER_H_
#define MANIFEST_HANDLERS_SPLASH_SCREEN_HANDLER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"

namespace wgt {
namespace parse {

enum class Orientation {
  DEFAULT,
  LANDSCAPE,
  PORTAIT
};

enum class ReadyWhen {
  FirstPaint,
  UserInteractive,
  Complete,
  Custom
};

struct SplashScreenData {
  Orientation orientation;
  std::string color;
  std::vector<std::string> background_color;
  std::vector<std::string> background_image;
  std::vector<std::string> image;
  std::vector<std::string> image_border;
};

class SplashScreenInfo : public parser::ManifestData {
 public:
  SplashScreenInfo();
  virtual ~SplashScreenInfo();

  void set_src(const std::string &src) { src_ = src; }
  const std::string& src() const { return src_; }
  void set_splash_screen_data(const SplashScreenData& splash_screen_data) {
    splash_screen_data_.push_back(splash_screen_data); }
  const std::vector<SplashScreenData>& splash_screen_data()
  { return splash_screen_data_; }
  void set_ready_when(ReadyWhen ready_when) { ready_when_ = ready_when; }
  const ReadyWhen ready_when() { return ready_when_; }

 private:
  std::string src_;
  std::vector<SplashScreenData> splash_screen_data_;
  ReadyWhen ready_when_;
};

class SplashScreenHandler : public parser::ManifestHandler {
 public:
  SplashScreenHandler() {}
  virtual ~SplashScreenHandler() {}
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  bool Validate(
      const parser::ManifestData& data,
      const parser::ManifestDataMap& handlers_output,
      std::string* error) const override;
  std::string Key() const override;

 private:
  bool ParseSingleOrientation(const parser::Manifest& manifest,
                              Orientation orientation,
                              SplashScreenInfo* ss_info);
  bool ParseElement(const parser::DictionaryValue* dict,
                    std::vector<std::string>* to_be_saved,
                    const char* keyToParse);
  bool ParseReadyWhen(const parser::Manifest& manifest,
                      SplashScreenInfo* ss_info);
  bool ParseColor(const parser::DictionaryValue* dict,
                  SplashScreenData* splash_screen);
};

}  // namespace parse
}  // namespace wgt

#endif  // MANIFEST_HANDLERS_SPLASH_SCREEN_HANDLER_H_
