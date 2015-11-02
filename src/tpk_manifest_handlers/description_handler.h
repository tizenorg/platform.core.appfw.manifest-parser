// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_DESCRIPTION_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_DESCRIPTION_HANDLER_H_

#include <memory>
#include <string>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class DescriptionInfo : public parser::ManifestData {
 public:
  /**
   * @brief key
   * @param key string
   */
  static std::string key();
  /**
   * @brief set_description sets description
   * @param description
   */
  void set_description(const std::string& description) {
    description_ = description;
  }
  /**
   * @brief set_xml_lang sets xml lang
   * @param xml_lang
   */
  void set_xml_lang(const std::string& xml_lang) {
    xml_lang_ = xml_lang;
  }
  /**
   * @brief description
   * @return description string
   */
  const std::string& description() const {
    return description_;
  }
  // NOTE: not in spec
  const std::string& xml_lang() const {
    return xml_lang_;
  }

 private:
  std::string description_;
  std::string xml_lang_;
};

/**
 * @brief The DescriptionHandler class
 *
 * Handler of tizen-manifest.xml for xml elements:
 *  <description>
 */
class DescriptionHandler : public parser::ManifestHandler {
 public:
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

#endif  // TPK_MANIFEST_HANDLERS_DESCRIPTION_HANDLER_H_
