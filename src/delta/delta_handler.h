// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef DELTA_DELTA_HANDLER_H_
#define DELTA_DELTA_HANDLER_H_

#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace delta {

LIBSCL_EXPORT_API extern const char kDeltaInfoKey[];

class LIBSCL_EXPORT_API DeltaInfo : public parser::ManifestData {
 public:
  void set_added(const std::vector<std::string>& added) {
    added_ = added;
  }
  const std::vector<std::string>& added() const {
    return added_;
  }
  void set_modified(const std::vector<std::string>& modified) {
    modified_ = modified;
  }
  const std::vector<std::string>& modified() const {
    return modified_;
  }
  void set_removed(const std::vector<std::string>& removed) {
    removed_ = removed;
  }
  const std::vector<std::string>& removed() const {
    return removed_;
  }

 private:
  std::vector<std::string> added_;
  std::vector<std::string> modified_;
  std::vector<std::string> removed_;
};

/**
 * @brief The DeltaHandler class
 *
 * Main handler for all tags in delta file
 */
class LIBSCL_EXPORT_API DeltaHandler : public parser::ManifestHandler {
 public:
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  std::string Key() const override;
};

}  // namespace delta

#endif  // DELTA_DELTA_HANDLER_H_
