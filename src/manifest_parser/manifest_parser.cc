// Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_parser/manifest_parser.h"

#include <memory>

#include "manifest_parser/manifest_constants.h"
#include "manifest_parser/manifest_parser_impl.h"
#include "manifest_parser/manifest_util.h"
#include "manifest_parser/permission_types.h"
#include "manifest_parser/values.h"

namespace bf = boost::filesystem;

namespace parser {

ManifestParser::ManifestParser(
    std::unique_ptr<ManifestHandlerRegistry> registry)
    : impl_(new ManifestParserImpl(std::move(registry))) {
}

ManifestParser::~ManifestParser() {
}

const std::string& ManifestParser::GetErrorMessage() {
  return impl_->GetErrorMessage();
}

bool ManifestParser::ParseManifest(const boost::filesystem::path& path) {
  return impl_->ParseManifest(path);
}

std::shared_ptr<const ManifestData> ManifestParser::GetManifestData(
    std::string key) {
  return impl_->GetManifestData(key);
}

}  // namespace parser
