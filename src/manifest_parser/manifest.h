// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_PARSER_MANIFEST_H_
#define MANIFEST_PARSER_MANIFEST_H_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <set>
#include <vector>

#include "manifest_parser/values.h"

namespace parser {

// Wraps the DictionaryValue form of application's manifest. Enforces access to
// properties of the manifest using ManifestFeatureProvider.
class Manifest {
 public:
  explicit Manifest(
      std::unique_ptr<DictionaryValue> value);
  ~Manifest();

  // Returns false and |error| will be non-empty if the manifest is malformed.
  // |warnings| will be populated if there are keys in the manifest that cannot
  // be specified by the application type.
  bool ValidateManifest(std::string* error) const;

  // These access the wrapped manifest value, returning false when the property
  // does not exist or if the manifest type can't access it.
  bool HasKey(const std::string& key) const;
  bool HasPath(const std::string& path) const;
  bool Get(const std::string& path, const Value** out_value) const;
  bool Get(const std::string& path, Value** out_value) const;
  bool GetBoolean(const std::string& path, bool* out_value) const;
  bool GetInteger(const std::string& path, int* out_value) const;
  bool CompareNamespace(const std::string& path, const std::string& comparedNamespace) const;

  // If the path is supported by i18n, we can get a locale string from
  // this two GetString function. The following is locale priority:
  // Application locale (locale get from system).                 | high
  // Default locale (defaultlocale attribute of widget element)
  // Unlocalized (the element without xml:lang attribute)
  // Auto ("en-us"(tizen is "en-gb") will be considered as a default)
  // First (the worst case we get the first element)              | low
  bool GetString(const std::string& path, std::string* out_value,
                 const std::string comparedNamespace = "http://www.w3.org/ns/widgets") const;

  bool GetDictionary(const std::string& path,
                     const DictionaryValue** out_value) const;
  bool GetList(const std::string& path,
               const ListValue** out_value) const;

  // Returns a new Manifest equal to this one, passing ownership to
  // the caller.
  Manifest* DeepCopy() const;

  // Returns true if this equals the |other| manifest.
  bool Equals(const Manifest* other) const;

  // Gets the underlying DictionaryValue representing the manifest.
  // Note: only use this when you KNOW you don't need the validation.
  const DictionaryValue* value() const { return data_.get(); }

 private:
  // Returns true if the application can specify the given |path|.
  bool CanAccessPath(const std::string& path) const;
  bool CanAccessKey(const std::string& key) const;

  // Unique package id for tizen platform
  std::string package_id_;

  // The underlying dictionary representation of the manifest.
  std::unique_ptr<DictionaryValue> data_;

  Manifest(const Manifest&) = delete;
  Manifest& operator=(const Manifest&) = delete;
};

}  // namespace parser

#endif  // MANIFEST_PARSER_MANIFEST_H_
