// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_METADATA_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_METADATA_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class MetaDataInfo : public parser::ManifestData {
 public:
  MetaDataInfo(const std::string& key,
               const std::string& val)
               : key_(key),
                 value_(val) {}
  ~MetaDataInfo() override {}

  const std::string& key() const {
    return key_;
  }
  const std::string& val() const {
    return value_;
  }

 private:
  std::string key_;
  std::string value_;
};

struct MetaDataInfoList : public parser::ManifestData {
  std::vector<MetaDataInfo> items;
};

class MetaDataHandler : public parser::ManifestHandler {
 public:
  MetaDataHandler() {}
  ~MetaDataHandler() override {}

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

#endif  // TPK_MANIFEST_HANDLERS_METADATA_HANDLER_H_
