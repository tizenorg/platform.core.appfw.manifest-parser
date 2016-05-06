// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef EXAMPLES_CUSTOM_HANDLERS_ITEM_HANDLER_H_
#define EXAMPLES_CUSTOM_HANDLERS_ITEM_HANDLER_H_

#include <manifest_parser/manifest.h>
#include <manifest_parser/manifest_handler.h>

#include <string>
#include <utility>

namespace example {

class ItemHandler : public parser::ManifestHandler {
 public:
  ItemHandler();
  ~ItemHandler();

  bool Parse(
        const parser::Manifest& manifest,
        std::shared_ptr<parser::ManifestData>* output,
        std::string* error) override;
  bool Validate(
        const parser::ManifestData& data,
        const parser::ManifestDataMap& handlers_output,
        std::string* error) const override;
  std::string Key() const override;
  bool AlwaysParseForKey() const override;
};

class ItemInfo : public parser::ManifestData {
 public:
  ItemInfo() {}
  ~ItemInfo() {}
  void set_type(std::string type) { type_ = std::move(type); }
  void set_value(std::string value) { value_ = std::move(value); }
  const std::string& type() const { return type_; }
  const std::string& value() const { return value_; }

 private:
  std::string type_;
  std::string value_;
};

}  // namespace example

#endif  // EXAMPLES_CUSTOM_HANDLERS_ITEM_HANDLER_H_
