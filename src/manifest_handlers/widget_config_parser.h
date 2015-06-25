// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef MANIFEST_HANDLERS_WIDGET_CONFIG_PARSER_H_
#define MANIFEST_HANDLERS_WIDGET_CONFIG_PARSER_H_

#include <boost/filesystem/path.hpp>

#include <memory>
#include <string>

#include "manifest_parser/manifest_parser.h"

namespace wgt {
namespace parse {

class WidgetConfigParser {
 public:
  WidgetConfigParser();

  std::shared_ptr<const parser::ManifestData> GetManifestData(
      const std::string& key);
  const std::string& GetErrorMessage() const;
  bool ParseManifest(const boost::filesystem::path& path);

 private:
  bool CheckStartFile();

  std::unique_ptr<parser::ManifestParser> parser_;
  boost::filesystem::path widget_path_;
  std::string error_;
};

}  // namespace parse
}  // namespace wgt

#endif  // MANIFEST_HANDLERS_WIDGET_CONFIG_PARSER_H_
