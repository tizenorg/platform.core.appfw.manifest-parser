// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef DELTA_DELTA_HANDLER_H_
#define DELTA_DELTA_HANDLER_H_

#include <set>
#include <string>

#include "manifest_parser/manifest_handler.h"

namespace delta {

extern const char kDeltaInfoKey[];

class DeltaInfo : public parser::ManifestData {
 public:
  void set_added(const std::set<std::string>& added) {
    added_ = added;
  }
  const std::set<std::string>& added() const {
    return added_;
  }
  void set_modified(const std::set<std::string>& modified) {
    modified_ = modified;
  }
  const std::set<std::string>& modified() const {
    return modified_;
  }
  void set_removed(const std::set<std::string>& removed) {
    removed_ = removed;
  }
  const std::set<std::string>& removed() const {
    return removed_;
  }

 private:
  std::set<std::string> added_;
  std::set<std::string> modified_;
  std::set<std::string> removed_;
};

/**
 * @brief The DeltaHandler class
 *
 * Main handler for all tags in delta file
 */
class DeltaHandler : public parser::ManifestHandler {
 public:
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  std::string Key() const override;
};

}  // namespace delta

#endif  // DELTA_DELTA_HANDLER_H_
