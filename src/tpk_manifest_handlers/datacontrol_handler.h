// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_DATACONTROL_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_DATACONTROL_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class DataControlInfo : public parser::ManifestData {
 public:
  DataControlInfo(const std::string& access,
                  const std::string& providerid,
                  const std::string& type)
                  : access_(access),
                    providerid_(providerid),
                    type_(type) {}
  ~DataControlInfo() override {}

  const std::string& access() const {
    return access_;
  }
  const std::string& providerid() const {
    return providerid_;
  }
  const std::string& type() const {
    return type_;
  }

 private:
  std::string access_;
  std::string providerid_;
  std::string type_;
};

struct DataControlInfoList : public parser::ManifestData {
  std::vector<DataControlInfo> items;
};

class DataControlHandler : public parser::ManifestHandler {
 public:
  DataControlHandler() {}
  ~DataControlHandler() override {}

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

#endif  // TPK_MANIFEST_HANDLERS_DATACONTROL_HANDLER_H_
