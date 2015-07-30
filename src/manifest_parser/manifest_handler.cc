// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_parser/manifest_handler.h"

#include <cassert>
#include <set>

namespace parser {

ManifestHandler::~ManifestHandler() {
}

bool ManifestHandler::Validate(
    const ManifestData& /*data*/,
    const ManifestDataMap& /*handlers_output*/,
    std::string* /*error*/) const {
  return true;
}

bool ManifestHandler::VerifyElementNamespace(const parser::Manifest& manifest,
                            const std::string& key_to_compare,
                            const std::string& desired_namespace_value) {
  parser::Value* value = nullptr;
  if (!manifest.Get(key_to_compare, &value))
    return true;  // skip check as element is not what we expect
  if (value->GetType() == parser::Value::TYPE_LIST) {
    parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    for (auto& value_ptr : *list) {
      std::string namespace_value;
      parser::DictionaryValue* dict = nullptr;
      if (value_ptr->GetAsDictionary(&dict)) {
        dict->GetString(kNamespaceKey,
                        &namespace_value);
        if (namespace_value != desired_namespace_value)
          return false;
      }
    }
  } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    std::string namespace_value;
    parser::DictionaryValue* dict = nullptr;
    if (value->GetAsDictionary(&dict)) {
      dict->GetString(kNamespaceKey,
                      &namespace_value);
      if (namespace_value != desired_namespace_value)
        return false;
    }
  } else {
    return true;  // skip check as element is not what we expect
  }

  // all check successfull
  return true;
}

bool ManifestHandler::AlwaysParseForType() const {
  return false;
}

bool ManifestHandler::AlwaysValidateForType() const {
  return false;
}

std::vector<std::string> ManifestHandler::PrerequisiteKeys() const {
  return std::vector<std::string>();
}

ManifestHandlerRegistry::ManifestHandlerRegistry() {
}

ManifestHandlerRegistry::ManifestHandlerRegistry(
    const std::vector<ManifestHandler*>& handlers) {
  for (std::vector<ManifestHandler*>::const_iterator it = handlers.begin();
       it != handlers.end(); ++it) {
    RegisterManifestHandler(*it);
  }

  ReorderHandlersGivenDependencies();
}

ManifestHandlerRegistry::~ManifestHandlerRegistry() {
}

void ManifestHandlerRegistry::RegisterManifestHandler(
    ManifestHandler* handler) {
  handlers_[handler->Key()] = handler;
  ReorderHandlersGivenDependencies();
}

ManifestHandlerMap ManifestHandlerRegistry::handlers() {
  return handlers_;
}

ManifestHandlerOrderMap
ManifestHandlerRegistry::get_manifest_handlers_order_map() {
  return order_map_;
}

void ManifestHandlerRegistry::ReorderHandlersGivenDependencies() {
  std::set<ManifestHandler*> unsorted_handlers;
  for (ManifestHandlerMap::const_iterator iter = handlers_.begin();
       iter != handlers_.end(); ++iter) {
    unsorted_handlers.insert(iter->second);
  }

  int order = 0;
  while (true) {
    std::set<ManifestHandler*> next_unsorted_handlers;
    for (std::set<ManifestHandler*>::const_iterator iter =
             unsorted_handlers.begin();
         iter != unsorted_handlers.end(); ++iter) {
      ManifestHandler* handler = *iter;
      const std::vector<std::string>& prerequisites =
          handler->PrerequisiteKeys();
      int unsatisfied = prerequisites.size();
      for (size_t i = 0; i < prerequisites.size(); ++i) {
        ManifestHandlerMap::const_iterator prereq_iter =
            handlers_.find(prerequisites[i]);
        assert(prereq_iter != handlers_.end() &&
               "Application manifest handler depends on unrecognized key ");
        // Prerequisite is in our map.
        if (order_map_.find(prereq_iter->second) != order_map_.end())
          unsatisfied--;
      }
      if (unsatisfied == 0) {
        order_map_[handler] = order;
        order++;
      } else {
        // Put in the list for next time.
        next_unsorted_handlers.insert(handler);
      }
    }
    if (next_unsorted_handlers.size() == unsorted_handlers.size())
      break;
    unsorted_handlers.swap(next_unsorted_handlers);
  }

  // If there are any leftover unsorted handlers, they must have had
  // circular dependencies.
  assert(unsorted_handlers.empty() &&
         "Application manifest handlers have circular dependencies!");
}

}  // namespace parser
