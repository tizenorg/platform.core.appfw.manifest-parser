// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_PROFILE_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_PROFILE_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class ProfileInfo : public parser::ManifestData {
 public:
  void AddProfile(const std::string& profile) {
    profiles_.push_back(profile);
  }

  const std::vector<std::string>& profiles() const {
    return profiles_;
  }

 private:
  std::vector<std::string> profiles_;
};

/**
 * @brief The ProfileHandler class
 *
 * Handler of tizen-manifest.xml for xml elements:
 *  <profile>
 */
class ProfileHandler : public parser::ManifestHandler {
 public:
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  std::string Key() const override;
};

}   // namespace parse
}   // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_PROFILE_HANDLER_H_
