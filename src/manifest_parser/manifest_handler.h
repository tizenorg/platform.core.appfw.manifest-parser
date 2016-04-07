// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_PARSER_MANIFEST_HANDLER_H_
#define MANIFEST_PARSER_MANIFEST_HANDLER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest.h"

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace parser {

// A base class for parsed manifest data that APIs want to store on
// the application.
class LIBSCL_EXPORT_API ManifestData {
 public:
  ManifestData() : exists_(true) {}
  virtual ~ManifestData() {}
  bool exists() const { return exists_; }
  void set_exists(bool exists) { exists_ = exists; }
 private:
  // This member should be set to false in case of there are no
  // elements for specific handler to be parsed in manifest
  // example:
  // <widget><application>app</application></widget>
  // in case of handling 'widget.application' exists_ should stay true
  // in case if handling 'widget.privilege' exists_ should be set to false
  bool exists_;
};

typedef std::map<std::string, std::shared_ptr<ManifestData> >
    ManifestDataMap;

class LIBSCL_EXPORT_API ManifestHandler {
 public:
  virtual ~ManifestHandler();

  // This function should parse values in manifest which are identified by
  // the key returned from Key() method.
  // If parsing is successfull then true should be returned and output pointer
  // set to valud ManifestDate derived class.
  // Otherwise (in case of error) function should return false value and
  // set error function argument to problem description.
  virtual bool Parse(
      const Manifest& manifest,
      std::shared_ptr<ManifestData>* output,
      std::string* error) = 0;

  // Returns false in case of failure and sets writes error message
  // in |error| if present.
  // This function is called if Parse() function will set non-empty output.
  // By default validation will just return true.
  virtual bool Validate(
      const ManifestData& data,
      const ManifestDataMap& handlers_output,
      std::string* error) const;

  // If false (the default), only parse the manifest if a registered
  // key is present in the manifest. If true, always attempt to parse
  // the manifest for this application type, even if no registered keys
  // are present. This allows specifying a default parsed value for
  // application that don't declare our key in the manifest.
  virtual bool AlwaysParseForKey() const;

  // The list of keys that, if present, should be parsed before calling our
  // Parse (typically, because our Parse needs to read those keys).
  // Defaults to empty.
  virtual std::vector<std::string> PrerequisiteKeys() const;

  // The keys to register handler for (in Register).
  virtual std::string Key() const = 0;
};

LIBSCL_EXPORT_API bool VerifyElementNamespace(const parser::DictionaryValue& dict,
                            const std::string& requested_namespace);
/**
 * @brief GetOneOrMany
 *        Provides vector of dictionary value pointers for given key
 *
 * This function is shortcut for query zero, one or more elements of given key.
 * Client code receives "unified" type - list of items so that it do not need
 * to check if underlaying values are lists or dictionaries.
 *
 * @param dict root dictionary where lookup should start
 * @param path path to be resolved in tree
 * @param namespace_prefix value of namespace which will be used to filter
 *                         elements
 * @return elements matching parameters
 */
LIBSCL_EXPORT_API const std::vector<const DictionaryValue*> GetOneOrMany(
    const DictionaryValue* dict, const std::string& path,
    const std::string& namespace_prefix);

typedef std::map<std::string, std::shared_ptr<ManifestHandler>>
    ManifestHandlerMap;
typedef std::map<std::shared_ptr<ManifestHandler>, int> ManifestHandlerOrderMap;

class LIBSCL_EXPORT_API ManifestHandlerRegistry {
 public:
  ManifestHandlerRegistry();
  explicit ManifestHandlerRegistry(
      const std::vector<ManifestHandler*>& handlers);
  ~ManifestHandlerRegistry();

  // Register a manifest handler for key, which is provided by Key() method
  // in ManifestHandler implementer. Registry takes ownership of registered
  // handler.
  void RegisterManifestHandler(ManifestHandler* handler);

  const ManifestHandlerMap& handlers() const;
  const ManifestHandlerOrderMap& get_manifest_handlers_order_map() const;

 private:
  // This function is sorting handlers as some of them might need to
  // be parsed before others are parsed - maybe it should be removed
  void ReorderHandlersGivenDependencies();

  ManifestHandlerMap handlers_;

  // Handlers are executed in order; lowest order first.
  ManifestHandlerOrderMap order_map_;
};

}  // namespace parser

#endif  // MANIFEST_PARSER_MANIFEST_HANDLER_H_
