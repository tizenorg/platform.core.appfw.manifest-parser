// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "custom_handlers/item_list_handler.h"

#include <string>

#include "custom_handlers/handler_constants.h"
#include "custom_handlers/item_handler.h"

namespace {

const char items_list_path[] = "widget.item-list";
const char item_element_name[] = "item";
const char value[] = "#text";
const char attribute[] = "@";
const char attribute_name[] = "type";
const char attribute_count[] = "count";

}  // namespace

namespace example {

ItemListHandler::ItemListHandler() {
}

ItemListHandler::~ItemListHandler() {
}

bool ItemListHandler::ParseItemElement(parser::DictionaryValue* item_element,
                                       std::shared_ptr<ItemInfo> info_output,
                                       std::string* error) {
  std::string item_value, item_type;
  if (item_element->GetString(std::string(attribute) + attribute_name,
                              &item_type))
    info_output->set_type(item_type);
  if (item_element->GetString(value, &item_value))
    info_output->set_value(item_value);
  return true;
}

bool ItemListHandler::Parse(const parser::Manifest& manifest,
                            std::shared_ptr<parser::ManifestData>* output,
                            std::string* error) {
  std::shared_ptr<ItemListInfo> item_list_info(new ItemListInfo);
  if (!manifest.HasPath(items_list_path)) {
    *error = std::string("Element: ") + items_list_path + "does not exist.";
    return false;
  }
  if (!manifest.HasPath(std::string(items_list_path) + "." + attribute
                        + attribute_count)) {
    *error = std::string("Element ") + items_list_path +
             " has no count attribute.";
    return false;
  }
  std::string count;
  if (!manifest.GetString(std::string(items_list_path) + "." + attribute
                           + attribute_count, &count)) {
    *error = std::string("Element ") + items_list_path + " exists, but has" +
             " no attribute named " + attribute_count;
    return false;
  }
  item_list_info->set_items_size(std::stoi(count));

  const parser::Value* val;
  if (!manifest.Get(std::string(items_list_path) + "." + item_element_name,
                    &val)) {
    *error = std::string("Can't access ") + items_list_path + " element.";
    return false;
  }
  const parser::ListValue* list_val;
  val->GetAsList(&list_val);
  for (const auto& item : *list_val) {
    parser::DictionaryValue* item_element;
    if (!item->GetAsDictionary(&item_element)) {
      *error = std::string("Element item in element ") + items_list_path +
               " exists, but can't access it.";
      return false;
    }
    std::shared_ptr<ItemInfo> info(new ItemInfo());
    if (!ParseItemElement(item_element, info, error)) {
      return false;
    }
    item_list_info->AppendItem(info);
  }
  *output = item_list_info;
  return true;
}

bool ItemListHandler::Validate(const parser::ManifestData& data,
                               const parser::ManifestDataMap& handlers_output,
                               std::string* error) const {
  const ItemListInfo& info = static_cast<const ItemListInfo&>(data);
  const int item_count = info.items_size();
  for (int i = 0; i < item_count; ++i) {
    if (!info.get_item(i) || info.get_item(i)->value().empty()) {
      *error = "Validation of item list manifest data failed.";
      return false;
    }
  }
  return true;
}

std::string ItemListHandler::Key() const {
  return keys::item_handler_list_key;
}

bool ItemListHandler::AlwaysParseForType() const {
  return true;
}

}  // namespace example
