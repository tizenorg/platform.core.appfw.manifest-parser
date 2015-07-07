// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef MANIFEST_HANDLERS_APPLICATION_ICONS_HANDLER_H_
#define MANIFEST_HANDLERS_APPLICATION_ICONS_HANDLER_H_

#include <string>
#include <vector>

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_parser/manifest_handler.h"

namespace wgt {
namespace parse {

class ApplicationIcon {
 public:
  explicit ApplicationIcon(const std::string& path, int height = -1,
                           int width = -1);

  bool GetWidth(int* width) const;
  bool GetHeight(int* height) const;
  const std::string& path() const;

  void set_width(int width);
  void set_height(int height);

 private:
  std::string path_;
  int height_;
  int width_;
};

class ApplicationIconsInfo : public parser::ManifestData {
 public:
  const std::vector<ApplicationIcon>& icons() const;
  void AddIcon(const ApplicationIcon& icon_path);
 private:
  std::vector<ApplicationIcon> icons_;
};

class ApplicationIconsHandler : public parser::ManifestHandler {
 public:
  ApplicationIconsHandler();
  ~ApplicationIconsHandler();

  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  bool AlwaysParseForType() const override;
  std::string Key() const override;
};

}  // namespace parse
}  // namespace wgt

#endif  // MANIFEST_HANDLERS_APPLICATION_ICONS_HANDLER_H_
