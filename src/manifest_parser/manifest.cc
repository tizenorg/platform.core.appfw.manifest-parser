// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_parser/manifest.h"

#include <algorithm>
#include <list>

#include "utils/logging.h"
#include "manifest_parser/manifest_constants.h"

namespace parser {

Manifest::Manifest(std::unique_ptr<DictionaryValue> value)
    : data_(std::move(value)) {
}

Manifest::~Manifest() {
}

bool Manifest::CompareNamespace(
    const std::string& path, const std::string comparedNamespace) const {
  std::string comparingNamespace;
  LOG(ERROR) << "about to check namespace";
  data_->GetString(path + ".@namespace", &comparingNamespace);
  return comparedNamespace == comparingNamespace;
}

bool Manifest::ValidateManifest(
    std::string* /*error*/) const {
  // TODO(xiang): support features validation
  return true;
}

bool Manifest::HasKey(const std::string& key) const {
  return CanAccessKey(key) && data_->HasKey(key);
}

bool Manifest::HasPath(const std::string& path) const {
  Value* ignored = Value::CreateNullValue();
  return CanAccessPath(path) && data_->Get(path, &ignored);
}

bool Manifest::Get(
    const std::string& path, const Value** out_value) const {
  return CanAccessPath(path) && data_->Get(path, out_value);
}

bool Manifest::Get(
    const std::string& path, Value** out_value) const {
  return this->Get(
      path,
      const_cast<const Value**>(out_value));
}

bool Manifest::GetBoolean(
    const std::string& path, bool* out_value) const {
  return CanAccessPath(path) && data_->GetBoolean(path, out_value);
}

bool Manifest::GetInteger(
    const std::string& path, int* out_value) const {
  return CanAccessPath(path) && data_->GetInteger(path, out_value);
}

bool Manifest::GetString(
    const std::string& path, std::string* out_value) const {

  if (!CompareNamespace(path) || !CanAccessPath(path))
    return false;

  return data_->GetString(path, out_value);
}

bool Manifest::GetDictionary(
    const std::string& path, const DictionaryValue** out_value) const {
  return CanAccessPath(path) && data_->GetDictionary(path, out_value);
}

bool Manifest::GetList(
    const std::string& path, const ListValue** out_value) const {
  return CanAccessPath(path) && data_->GetList(path, out_value);
}

Manifest* Manifest::DeepCopy() const {
  Manifest* manifest = new Manifest(
      std::unique_ptr<DictionaryValue>(data_->DeepCopy()));
  return manifest;
}

bool Manifest::Equals(const Manifest* other) const {
  return other && data_->Equals(other->value());
}

bool Manifest::CanAccessPath(const std::string& /*path*/) const {
  return true;
}

bool Manifest::CanAccessKey(const std::string& /*key*/) const {
  return true;
}

}  // namespace parser
