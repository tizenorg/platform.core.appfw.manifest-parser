// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "manifest_parser/manifest_parser_impl.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "manifest_parser/manifest_constants.h"
#include "manifest_parser/manifest_util.h"
#include "manifest_parser/permission_types.h"
#include "manifest_parser/values.h"

namespace bf = boost::filesystem;

namespace {

typedef std::string LocalError;

void SetError(const std::string& message, std::string* error) {
  *error = message;
}

void SetError(const std::string& message, const std::string& arg,
    std::string* error) {
  *error = message + arg;
}

}  // namespace

namespace parser {

ManifestParserImpl::ManifestParserImpl(
    std::unique_ptr<ManifestHandlerRegistry> registry)
    : valid_(false) {
  registry_ = std::move(registry);
}

ManifestParserImpl::~ManifestParserImpl() {
}

const std::string& ManifestParserImpl::GetErrorMessage() const {
  return error_;
}

bool ManifestParserImpl::ParseManifest(const bf::path& manifest_path) {
  error_.clear();
  if (manifest_path.empty()) {
    SetError(application_manifest_errors::kErrMsgNoPath, &error_);
    return false;
  }
  if (!exists(manifest_path)) {
    SetError(application_manifest_errors::kErrMsgInvalidPath, &error_);
    return false;
  }

  manifest_ = parser::LoadManifest(manifest_path.string(), &error_);
  if (!manifest_.get())
    return false;
  if (!ParseManifestData(&error_))
    return false;
  if (!ValidateAppManifest(&error_))
    return false;

  valid_ = error_.empty();
  return valid_;
}

bool ManifestParserImpl::ValidateAppManifest(std::string* error) {
  const ManifestHandlerMap& handlers = registry_->handlers();
  for (auto p : handlers) {
    ManifestHandler* handler = p.second;
    if ((manifest_data_.find(p.first) != manifest_data_.end() ||
         handler->AlwaysValidateForType()) &&
        !handler->Validate(*GetManifestData(handler->Key()).get(),
            manifest_data_, error))
      return false;
  }
  return true;
}

bool ManifestParserImpl::ParseAppManifest(std::string* error) {
  std::map<int, ManifestHandler*> handlers_by_order;
  const ManifestHandlerMap& handlers = registry_->handlers();
  ManifestHandlerOrderMap order_map =
      registry_->get_manifest_handlers_order_map();
  for (auto p : handlers) {
    ManifestHandler* handler = p.second;
    if (manifest_->HasPath(p.first) ||
        handler->AlwaysParseForType()) {
      handlers_by_order[order_map[handler]] = handler;
    }
  }
  for (auto handler : handlers_by_order) {
    std::shared_ptr<ManifestData> output;
    if (handler.second->Parse(*manifest_.get(), &output, error)) {
      if (output)
        manifest_data_[handler.second->Key()] = output;
    } else {
      return false;
    }
  }
  return true;
}

bool ManifestParserImpl::ParseManifestData(std::string* error) {
  assert(error);
  if (!ParseAppManifest(error))
    return false;
  return true;
}

void ManifestParserImpl::SetManifestData(const std::string& key,
    std::shared_ptr<ManifestData> data) {
  manifest_data_[key] = data;
}

std::shared_ptr<const ManifestData> ManifestParserImpl::GetManifestData(
    const std::string& key) {
  ManifestDataMap::const_iterator iter = manifest_data_.find(key);
  if (iter != manifest_data_.end())
    return iter->second;
  return nullptr;
}

std::shared_ptr<ManifestData> ManifestParserImpl::AccessManifestData(
    const std::string& key) {
  ManifestDataMap::const_iterator iter = manifest_data_.find(key);
  if (iter != manifest_data_.end())
    return iter->second;
  return nullptr;
}

void ManifestParserImpl::EraseManifestData(const std::string& key) {
  ManifestDataMap::iterator iter = manifest_data_.find(key);
  if (iter != manifest_data_.end())
    manifest_data_.erase(iter);
}

}  // namespace parser


