// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_PACKAGE_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_PACKAGE_HANDLER_H_

#include <memory>
#include <string>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

class PackageInfo : public parser::ManifestData {
 public:
  void set_xmlns(const std::string& xmlns) {
    xmlns_ = xmlns;
  }
  void set_api_version(const std::string& api_version) {
    api_version_ = api_version;
  }
  void set_package(const std::string& package) {
    package_ = package;
  }
  void set_version(const std::string& version) {
    version_ = version;
  }
  void set_install_location(const std::string& install_location) {
    install_location_ = install_location;
  }

  const std::string& xmlns() const {
    return xmlns_;
  }
  const std::string& api_version() const {
    return api_version_;
  }
  const std::string& package() const {
    return package_;
  }
  const std::string& version() const {
    return version_;
  }
  const std::string& install_location() const {
    return install_location_;
  }


 private:
  std::string xmlns_;
  std::string api_version_;
  std::string package_;
  std::string version_;
  std::string install_location_;
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
