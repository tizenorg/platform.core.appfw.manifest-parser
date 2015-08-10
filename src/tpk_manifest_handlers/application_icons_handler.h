// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_APPLICATION_ICONS_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_APPLICATION_ICONS_HANDLER_H_

#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class ApplicationIcon {
 public:
  explicit ApplicationIcon(const std::string& path)
                          : path_(path) {}

const std::string& path() const {
  return path_;
}

 private:
  std::string path_;
};

class ApplicationIconsInfo : public parser::ManifestData {
 public:
  const std::vector<ApplicationIcon>& icons() const {
  return icons_;
}

  void AddIcon(const ApplicationIcon& icon_path);

 private:
  std::vector<ApplicationIcon> icons_;
};

class ApplicationIconsHandler : public parser::ManifestHandler {
 public:
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  std::string Key() const override;
};

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_APPLICATION_ICONS_HANDLER_H_
