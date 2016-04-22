// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_parser/manifest_handler.h"

#include <cassert>
#include <set>

namespace {

const char kNamespaceKey[] = "@namespace";

}

namespace parser {

ManifestHandler::~ManifestHandler() {
}

bool ManifestHandler::Validate(
    const ManifestData& /*data*/,
    const ManifestDataMap& /*handlers_output*/,
    std::string* /*error*/) const {
  return true;
}

bool VerifyElementNamespace(
    const parser::DictionaryValue& dict,
    const std::string& requested_namespace) {
  std::string element_namespace;
  if (!dict.GetString(kNamespaceKey, &element_namespace))
    return false;
  return element_namespace == requested_namespace;
}

const std::vector<const DictionaryValue*> GetOneOrMany(
    const DictionaryValue* dict, const std::string& path,
    const std::string& namespace_prefix) {
  std::vector<const DictionaryValue*> nodes;
  const parser::Value* value = nullptr;
  if (dict->Get(path, &value)) {
    if (value->GetType() == parser::Value::TYPE_LIST) {
      const parser::ListValue* list = nullptr;
      value->GetAsList(&list);
      for (auto& item : *list) {
        const parser::DictionaryValue* dict = nullptr;
        item->GetAsDictionary(&dict);
        if (!namespace_prefix.empty() &&
            !VerifyElementNamespace(*dict, namespace_prefix))
          continue;
        nodes.push_back(dict);
      }
    } else if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
      const parser::DictionaryValue* dict = nullptr;
      value->GetAsDictionary(&dict);
      if (namespace_prefix.empty() ||
          VerifyElementNamespace(*dict, namespace_prefix)) {
        nodes.push_back(dict);
      }
    } else {
      assert(false &&
          "This should not happen. Only element node can be resolved.");
    }
  }
  return nodes;
}

bool ManifestHandler::AlwaysParseForKey() const {
  return false;
}

std::vector<std::string> ManifestHandler::PrerequisiteKeys() const {
  return std::vector<std::string>();
}

ManifestHandlerRegistry::ManifestHandlerRegistry() {
}

ManifestHandlerRegistry::ManifestHandlerRegistry(
    const std::vector<std::shared_ptr<ManifestHandler>>& handlers) {
  for (auto it = handlers.begin(); it != handlers.end(); ++it) {
    handlers_[(*it)->Key()] = *it;
  }

  ReorderHandlersGivenDependencies();
}

ManifestHandlerRegistry::~ManifestHandlerRegistry() {
}

void ManifestHandlerRegistry::RegisterManifestHandler(
    ManifestHandler* handler) {
  handlers_[handler->Key()] = std::shared_ptr<ManifestHandler>(handler);
  ReorderHandlersGivenDependencies();
}

const ManifestHandlerMap& ManifestHandlerRegistry::handlers() const {
  return handlers_;
}

const ManifestHandlerOrderMap&
ManifestHandlerRegistry::get_manifest_handlers_order_map() const {
  return order_map_;
}

void ManifestHandlerRegistry::ReorderHandlersGivenDependencies() {
  std::set<std::shared_ptr<ManifestHandler>> unsorted_handlers;
  for (ManifestHandlerMap::const_iterator iter = handlers_.begin();
       iter != handlers_.end(); ++iter) {
    unsorted_handlers.insert(iter->second);
  }

  int order = 0;
  while (true) {
    std::set<std::shared_ptr<ManifestHandler>> next_unsorted_handlers;
    for (std::set<std::shared_ptr<ManifestHandler>>::const_iterator iter =
             unsorted_handlers.begin();
         iter != unsorted_handlers.end(); ++iter) {
      auto handler = *iter;
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
