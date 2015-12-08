// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_PACKAGE_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_PACKAGE_HANDLER_H_

#include <memory>
#include <string>
#include <utility>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

using LangTextPair = std::pair<std::string, std::string>;

class PackageInfo : public parser::ManifestData {
 public:
  /**
   * @brief key
   * @param key string
   */
  static std::string key();
  /**
   * @brief set_xmlns xmlns
   * @param xmlns
   */
  void set_xmlns(const std::string& xmlns) {
    xmlns_ = xmlns;
  }
  /**
   * @brief set_api_version set api version
   * @param api_version
   */
  void set_api_version(const std::string& api_version) {
    api_version_ = api_version;
  }
  /**
   * @brief set_package set package
   * @param package
   */
  void set_package(const std::string& package) {
    package_ = package;
  }
  /**
   * @brief set_version sets version
   * @param version
   */
  void set_version(const std::string& version) {
    version_ = version;
  }
  /**
   * @brief set_install_location sets install location
   * @param install_location
   */
  void set_install_location(const std::string& install_location) {
    install_location_ = install_location;
  }

  void set_nodisplay_setting(const std::string& nodisplay_setting) {
    nodisplay_setting_ = nodisplay_setting;
  }
  void AddLabel(const std::string& lang, const std::string& text) {
    labels_.emplace_back(lang, text);
  }

  /**
   * @brief xmlns
   * @return xmlns string
   */
  const std::string& xmlns() const {
    return xmlns_;
  }
  /**
   * @brief api_version
   * @return  api_version string
   */
  const std::string& api_version() const {
    return api_version_;
  }
  /**
   * @brief package
   * @return package string
   */
  const std::string& package() const {
    return package_;
  }
  /**
   * @brief version
   * @return version string
   */
  const std::string& version() const {
    return version_;
  }
  const std::string& install_location() const {
    return install_location_;
  }
  const std::string& nodisplay_setting() const {
    return nodisplay_setting_;
  }
  const std::vector<LangTextPair>& labels() const {
    return labels_;
  }

 private:
  std::string xmlns_;
  std::string api_version_;
  std::string package_;
  std::string version_;
  std::string install_location_;
  std::string nodisplay_setting_;
  std::vector<LangTextPair> labels_;
};

/**
 * @brief The PackageHandler class
 *
 * Handler of tizen-manifest.xml for root xml element <manifest>
 * Child elements are parsed in other handlers.
 */
class PackageHandler : public parser::ManifestHandler {
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

#endif  // TPK_MANIFEST_HANDLERS_PACKAGE_HANDLER_H_
