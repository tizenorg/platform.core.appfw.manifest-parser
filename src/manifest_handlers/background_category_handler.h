// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef SRC_MANIFEST_HANDLERS_BACKGROUND_CATEGORY_HANDLER_H_
#define SRC_MANIFEST_HANDLERS_BACKGROUND_CATEGORY_HANDLER_H_

#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"

namespace wgt {
namespace parse {

/**
 * \brief Holds details about background-category element
 *
 * Purpose of this class is to hold information declared in background-category
 * element in manifest xml document
 */
class BackgroundCategoryInfo : public parser::ManifestData {
 public:
  /**
   * \brief Parametrized constructor
   *
   * \param value read from value attribute of background-category element
   */
  BackgroundCategoryInfo(const std::string& value);
  /**
   * \brief Destructor
   */
  virtual ~BackgroundCategoryInfo() {}

  /**
   * \brief Accessor method of value member
   *
   * \return reference to string that hold value read from value attribute of
   *         background-category element
   */
  const std::string& value() const { return value_; }

 private:
  std::string value_;
};

/**
 * \brief Container for detailed information of each declaration of
 *        background-category element
 */
struct BackgroundCategoryInfoList : public parser::ManifestData {
  /**
   * \brief Member that hold
   */
  std::vector<BackgroundCategoryInfo> background_categories;
};

/**
 * \brief The BackgroundCategoryHandler class
 *
 * Handler of config.xml for xml elements:
 *  - <tizen:background-category>.
 */
class BackgroundCategoryHandler : public parser::ManifestHandler {
 public:
  /**
   * \brief Parse information about background-category from manifest xml
   *        document and put it in BackgroundCategoryInfo
   *
   * \param manifest provide access to manifest xml document
   * \param output return parsed data of all background-category elements
   * \param error return error message
   */
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
