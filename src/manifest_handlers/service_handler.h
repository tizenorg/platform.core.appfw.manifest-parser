// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_HANDLERS_SERVICE_HANDLER_H_
#define MANIFEST_HANDLERS_SERVICE_HANDLER_H_

#include <string>
#include <utility>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"

namespace wgt {
namespace parse {

using LangNameVector = std::vector<std::pair<std::string, std::string>>;
using KeyValueVector = std::vector<std::pair<std::string, std::string>>;

class ServiceInfo {
 public:
  explicit ServiceInfo(const std::string& id, bool auto_restart = false,
      bool on_boot = false);
  virtual ~ServiceInfo();

  const std::string& id() const { return id_; }
  void set_id(const std::string& id) {
    id_ = id;
  }

  bool auto_restart() const { return auto_restart_; }
  void set_auto_restart(bool auto_restart) {
    auto_restart_ = auto_restart;
  }

  bool on_boot() const { return on_boot_; }
  void set_on_boot(bool on_boot) {
    on_boot_ = on_boot;
  }

  const LangNameVector& names() const {
    return names_;
  }
  void set_names(const LangNameVector& names) {
    names_ = names;
  }

  const std::string& icon() const {
    return icon_;
  }
  void set_icon(const std::string& icon) {
    icon_ = icon;
  }

  const std::string& content() const {
    return content_;
  }
  void set_content(const std::string& content) {
    content_ = content;
  }

  const std::string& description() const {
    return description_;
  }
  void set_description(const std::string& description) {
    description_ = description;
  }

  const std::vector<std::string>& categories() const {
    return categories_;
  }
  void set_categories(const std::vector<std::string>& categories) {
    categories_ = categories;
  }

  const KeyValueVector& metadata_set() const {
    return metadata_set_;
  }
  void set_metadata_set(const KeyValueVector& metadata_set) {
    metadata_set_ = metadata_set;
  }

 private:
  std::string id_;
  bool auto_restart_;
  bool on_boot_;
  LangNameVector names_;
  std::string icon_;
  std::string content_;
  std::string description_;
  std::vector<std::string> categories_;
  KeyValueVector metadata_set_;
};

struct ServiceList : public parser::ManifestData {
  std::vector<ServiceInfo> services;
};

/**
 * @brief The ServiceHandler class
 *
 * Handler of config.xml for xml elements:
 *  - <tizen:service>.
 */
class ServiceHandler : public parser::ManifestHandler {
 public:
  ServiceHandler();
  virtual ~ServiceHandler();

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

}  // namespace parse
}  // namespace wgt

#endif  // MANIFEST_HANDLERS_SERVICE_HANDLER_H_
