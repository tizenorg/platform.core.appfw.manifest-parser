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

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace parser {

class ManifestParserImpl;

class LIBSCL_EXPORT_API ManifestParser final {
 public:
  // This class should take ownership of ManifestHandlerRegistry
  explicit ManifestParser(std::unique_ptr<ManifestHandlerRegistry> registry);
  ~ManifestParser();

  virtual const std::string& GetErrorMessage() const;

  // Parses manifest file to manfiest_ object
  bool ParseManifest(const boost::filesystem::path& path);

  std::shared_ptr<const ManifestData> GetManifestData(const std::string& key);
  // Getter returning writable data so that value of key can be changed
  // used in WidgetConfigParser
  std::shared_ptr<ManifestData> AccessManifestData(const std::string& key);
  // Removes data by given key
  void EraseManifestData(const std::string& key);

  std::unique_ptr<ManifestParserImpl> impl_;
};

}  // namespace parser

#endif  // MANIFEST_PARSER_MANIFEST_PARSER_H_
