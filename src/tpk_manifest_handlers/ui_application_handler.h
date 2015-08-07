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

namespace tpk {
namespace parse {

class UIApplicationInfo : public parser::ManifestData {
 public:
  UIApplicationInfo(const std::string& appid,
                    const std::string& exec,
                    const std::string& multiple,
                    const std::string& nodisplay,
                    const std::string& taskmanage,
                    const std::string& type)
                    : appid_(appid),
                      exec_(exec),
                      multiple_(multiple),
                      nodisplay_(nodisplay),
                      taskmanage_(taskmanage),
                      type_(type) {}
  ~UIApplicationInfo() override {}

  const std::string& appid() const {
    return appid_;
  }
  const std::string& exec() const {
    return exec_;
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
  std::string multiple_;
  std::string nodisplay_;
  std::string taskmanage_;
  std::string type_;
};

struct UIApplicationInfoList : public parser::ManifestData {
  std::vector<UIApplicationInfo> items;
};

class UIApplicationHandler : public parser::ManifestHandler {
 public:
  UIApplicationHandler() {}
  ~UIApplicationHandler() override {}

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

#endif  // TPK_MANIFEST_HANDLERS_UI_APPLICATION_HANDLER_H_
