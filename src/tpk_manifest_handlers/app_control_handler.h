// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_APP_CONTROL_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_APP_CONTROL_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class AppControlInfo : public parser::ManifestData {
 public:
  AppControlInfo(const std::string& operation,
                 const std::string& uri,
                 const std::string& mime)
                 : operation_(operation),
                   uri_(uri),
                   mime_(mime) {}
  ~AppControlInfo() override {}

  const std::string& operation() const {
    return operation_;
  }

  const std::string& uri() const {
    return uri_;
  }

  const std::string& mime() const {
    return mime_;
  }

 private:
  std::string operation_;
  std::string uri_;
  std::string mime_;
};

struct AppControlInfoList : public parser::ManifestData {
  std::vector<AppControlInfo> items;
};

class AppControlHandler : public parser::ManifestHandler {
 public:
  AppControlHandler() {}
  ~AppControlHandler() override {}

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

#endif  // TPK_MANIFEST_HANDLERS_APP_CONTROL_HANDLER_H_
