// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef MANIFEST_PARSER_MANIFEST_PARSER_IMPL_H_
#define MANIFEST_PARSER_MANIFEST_PARSER_IMPL_H_

#include <boost/filesystem/path.hpp>
#include <pkgmgr/pkgmgr_parser.h>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "manifest_parser/manifest.h"
#include "manifest_parser/manifest_handler.h"

namespace parser {

class ManifestParserImpl {
 public:
  explicit ManifestParserImpl(
      std::unique_ptr<ManifestHandlerRegistry> registry);
  ~ManifestParserImpl();

  const std::string& GetErrorMessage();

  bool ParseManifest(const boost::filesystem::path& path);

  std::shared_ptr<const ManifestData> GetManifestData(std::string key);

 private:
  bool ParseManifestData(std::string* error);

  void SetManifestData(const std::string& key,
     std::shared_ptr<ManifestData> data);

  // Uses manifest handlers for parsing ManifestData stucts
  bool ParseAppManifest(std::string* error);
  // Uses manifest handlers for validating ManifestData stucts
  bool ValidateAppManifest(std::string* error);

  std::unique_ptr<ManifestHandlerRegistry> registry_;
  std::shared_ptr<Manifest> manifest_;
  ManifestDataMap manifest_data_;
  bool valid_;

  std::string error_;

  // For testing
  friend class ServiceHandlerTest;
};

}  // namespace parser

#endif  // MANIFEST_PARSER_MANIFEST_PARSER_IMPL_H_
