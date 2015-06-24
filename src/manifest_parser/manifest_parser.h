// Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_PARSER_MANIFEST_PARSER_H_
#define MANIFEST_PARSER_MANIFEST_PARSER_H_

#include <boost/filesystem/path.hpp>

#include <memory>
#include <string>

#include "manifest_parser/manifest.h"
#include "manifest_parser/manifest_handler.h"

namespace parser {

class ManifestParserImpl;

class ManifestParser final {
 public:
  // This class should take ownership of ManifestHandlerRegistry
  explicit ManifestParser(std::unique_ptr<ManifestHandlerRegistry> registry);
  ~ManifestParser();

  virtual const std::string& GetErrorMessage() const;

  // Parses manifest file to manfiest_ object
  bool ParseManifest(const boost::filesystem::path& path);

  std::shared_ptr<const ManifestData> GetManifestData(std::string key);

  std::unique_ptr<ManifestParserImpl> impl_;
};

}  // namespace parser

#endif  // MANIFEST_PARSER_MANIFEST_PARSER_H_
