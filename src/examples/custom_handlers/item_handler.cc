// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "custom_handlers/item_handler.h"

#include "custom_handlers/handler_constants.h"

namespace {

const char item_path[] = "widget.item";
const char value[] = ".#text";
const char attribute[] = ".@";
const char attribute_name[] = "type";

}  // namespace

namespace example {

ItemHandler::ItemHandler() {
}

ItemHandler::~ItemHandler() {
}

/*
 * According to defined config.xml file, the layout of manifest as
 * DictionaryValue is:
 *
 * "widget" : {
 *   "application" : {
 *     "@id" : "nNBDOItqjN.sample",
 *     "@package" : "nNBDOItqjN",
 *     "@required_version" : "2.2"
 *   }
 *   "item-list" : {
 *     "@count" : "3",
 *     "item" : {
 *       "@type" : "text",
 *       "#text" : "first"
 *     }
 *     "item" : {
 *       "@type" : "file",
 *       "#text" : "second.png"
 *     }
 *     "item" : {
 *       "@type" : "text",
 *       "#text" : "third"
 *     }
 *   }
 *   "item" : {
 *     "@type" : "additional"
 *     "#text" : "standalone"
 *   }
 * }
 *
 */
bool ItemHandler::Parse(const parser::Manifest& manifest,
                        std::shared_ptr<parser::ManifestData>* output,
                        std::string* error) {
  std::shared_ptr<ItemInfo> info(new ItemInfo);
  // error case (item element is not optional)
  if (!manifest.HasPath(item_path)) {
    *error = std::string("No element of type ") + item_path + ".";
    return false;
  }
  std::string item_value, item_type;
  if (!manifest.GetString(std::string(item_path) + value, &item_value)) {
    *error = std::string("Element of type ") + item_path +
             " exists, but has no value.";
    return false;
  }
  if (!manifest.GetString(
          std::string(item_path) + attribute + attribute_name, &item_type)) {
    *error = std::string("Attribute ") + attribute_name + " of element "
             + item_path + " does not exist.";
    return false;
  }
  info->set_value(item_value);
  info->set_type(item_type);
  *output = info;
  return true;
}

bool ItemHandler::Validate(const parser::ManifestData& data,
                           const parser::ManifestDataMap& handlers_output,
                           std::string* error) const {
  const ItemInfo& info = static_cast<const ItemInfo&>(data);
  if (info.type().empty() || info.value().empty()) {
    *error = "Validation of item manifest data failed.";
    return false;
  }
  return true;
}

std::string ItemHandler::Key() const {
  return keys::item_handler_key;
}

bool ItemHandler::AlwaysParseForType() const {
  return true;
}

}  // namespace example
