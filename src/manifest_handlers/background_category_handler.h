// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef SRC_MANIFEST_HANDLERS_BACKGROUND_CATEGORY_HANDLER_H_
#define SRC_MANIFEST_HANDLERS_BACKGROUND_CATEGORY_HANDLER_H_

#include <string>
#include <vector>

namespace wgt {
namespace parse {

class BackgroundCategoryInfo : public parser::ManifestData {
 public:
  BackgroundCategoryInfo(const std::string& value) {}
  virtual ~BackgroundCategoryInfo() {}

  void set_value(const std::string& value) {
    value_ = value;
  }

  const std::string& value() const { return value_; }

 private:
  std::string value_;
};

struct BackgroundCategoryInfoList : public parser::ManifestData {
  std::vector<BackgroundCategoryInfo> background_categories;
};

/**
 * @brief The BackgroundCategoryHandler class
 *
 * Handler of config.xml for xml elements:
 *  - <tizen:background-category>.
 */
class BackgroundCategoryHandler : public parser::ManifestHandler {
 public:
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  std::string Key() const override;

 private:
  bool ParseBackgroundCategoryElement(
      const parser::DictionaryValue& element_dict,
      BackgroundCategoryInfoList* bclist);
};

} // namespace parse
} // namespace wgt

#endif /* SRC_MANIFEST_HANDLERS_BACKGROUND_CATEGORY_HANDLER_H_ */
