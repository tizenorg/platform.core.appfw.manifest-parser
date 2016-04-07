// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_WATCH_APPLICATION_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_WATCH_APPLICATION_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"
#include "tpk_manifest_handlers/common/application_handler.h"

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace tpk {
namespace parse {

class LIBSCL_EXPORT_API WatchApplicationInfo : public ApplicationInfo {
 public:
  WatchApplicationInfo();

  /**
   * @brief Key indicating element name in tizen-manifest.xml file.
   * @param key string
   */
  static std::string key();

  /**
   * @brief Indicates whether the application draws the ambient mode UI itself.
   * @return ambient_support_ string
   */
  const std::string& ambient_support() const {
    return ambient_support_;
  }

  /**
   * @brief Watch application icon image.
   * @return icon string
   */
  const std::string& icon() const {
    return icon_;
  }

  /**
   * @brief Watch application text.
   * @return label string
   */
  const std::string& label() const {
    return label_;
  }

  /**
   * @brief Watch application type.
   * @return type string
   */
  const std::string& type() const {
    return type_;
  }

  void set_ambient_support(const std::string& ambient_support) {
    ambient_support_ = ambient_support;
  }

  void set_icon(const std::string& icon) {
    icon_ = icon;
  }

  void set_label(const std::string& label) {
    label_ = label;
  }
  void set_type(const std::string& type) {
    type_ = type;
  }

 private:
  std::string type_;
  std::string ambient_support_;
  std::string icon_;
  std::string label_;
};

struct LIBSCL_EXPORT_API WatchApplicationSingleEntry :
    public ApplicationSingleEntry<WatchApplicationInfo> {
  ApplicationImagesInfo app_images;
};

using WatchApplicationInfoList =
    ApplicationInfoList<WatchApplicationSingleEntry>;

/**
 * @brief The WatchApplicationHandler class
 *
 * Handler of tizen-manifest.xml for xml elements:
 *  <watch-application>
 *  \_  <label>
 *  \_  <icon>
 */
class LIBSCL_EXPORT_API WatchApplicationHandler : public parser::ManifestHandler {
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
  std::vector<std::string> PrerequisiteKeys() const override;
};

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_WATCH_APPLICATION_HANDLER_H_
