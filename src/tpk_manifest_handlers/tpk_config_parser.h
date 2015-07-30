// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_TPK_CONFIG_PARSER_H_
#define TPK_MANIFEST_HANDLERS_TPK_CONFIG_PARSER_H_

#include <boost/filesystem/path.hpp>

#include <memory>
#include <string>

#include "manifest_parser/manifest_parser.h"

namespace tpk {
namespace parse {

class TPKConfigParser {
 public:
  TPKConfigParser();

  std::shared_ptr<const parser::ManifestData> GetManifestData(
      const std::string& key);
  const std::string& GetErrorMessage() const;
  bool ParseManifest(const boost::filesystem::path& path);

 private:
  std::unique_ptr<parser::ManifestParser> parser_;
  std::string error_;
};

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_TPK_CONFIG_PARSER_H_
