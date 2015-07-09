// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef EXAMPLES_CUSTOM_HANDLERS_ITEM_LIST_HANDLER_H_
#define EXAMPLES_CUSTOM_HANDLERS_ITEM_LIST_HANDLER_H_

#include <manifest_parser/manifest.h>
#include <manifest_parser/manifest_handler.h>
#include <manifest_parser/values.h>

#include <string>
#include <vector>

#include "custom_handlers/item_handler.h"

namespace example {

class ItemListHandler : public parser::ManifestHandler {
 public:
  ItemListHandler();
  ~ItemListHandler();
  bool ParseItemElement(parser::DictionaryValue* item_element,
                        std::shared_ptr<ItemInfo> info_output,
                        std::string* error);
  bool Parse(
        const parser::Manifest& manifest,
        std::shared_ptr<parser::ManifestData>* output,
        std::string* error) override;
  bool Validate(
        const parser::ManifestData& data,
        const parser::ManifestDataMap& handlers_output,
        std::string* error) const override;
  std::string Key() const override;
  bool AlwaysParseForType() const override;
};

class ItemListInfo : public parser::ManifestData {
  int items_size_;
  std::vector<std::shared_ptr<ItemInfo>> items_;

 public:
  ItemListInfo() : items_size_(0) {}
  ~ItemListInfo() {}
  void AppendItem(std::shared_ptr<ItemInfo> info) {
    items_.push_back(info);
  }
  std::shared_ptr<ItemInfo> get_item(int idx) const { return items_[idx]; }
  void set_items_size(int items_size) { items_size_ = items_size; }
  int items_size() const { return items_size_; }
};

}  // namespace example

#endif  // EXAMPLES_CUSTOM_HANDLERS_ITEM_LIST_HANDLER_H_
