// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "delta/delta_parser.h"

#include <vector>

#include "delta/delta_handler.h"
#include "manifest_parser/manifest_handler.h"

namespace delta {

DeltaParser::DeltaParser() {
  std::vector<parser::ManifestHandler*> handlers = {
    new DeltaHandler,
  };

  std::unique_ptr<parser::ManifestHandlerRegistry> registry(
      new parser::ManifestHandlerRegistry(handlers));

  parser_.reset(new parser::ManifestParser(std::move(registry)));
}

std::shared_ptr<const parser::ManifestData> DeltaParser::GetManifestData(
    const std::string& key) {
  return parser_->GetManifestData(key);
}

const std::string& DeltaParser::GetErrorMessage() const {
  if (!error_.empty())
    return error_;
  return parser_->GetErrorMessage();
}

bool DeltaParser::ParseManifest(const boost::filesystem::path& path) {
  if (!parser_->ParseManifest(path))
    return false;

  return true;
}

}  // namespace delta
