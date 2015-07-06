// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_HANDLERS_CONTENT_HANDLER_H_
#define MANIFEST_HANDLERS_CONTENT_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"

namespace wgt {
namespace parse {

class ContentInfo : public parser::ManifestData {
 public:
  ContentInfo()
     : is_tizen_content_(false) { }

  virtual ~ContentInfo() { }

  const std::string& src() const {
    return src_;
  }
  const std::string& type() const {
    return type_;
  }
  const std::string& encoding() const {
    return encoding_;
  }
  void set_src(const std::string& src) { src_ = src; }
  void set_type(const std::string& type) { type_ = type; }
  void set_encoding(const std::string& encoding) { encoding_ = encoding; }
  bool is_tizen_content() const {
    return is_tizen_content_;
  }
  void set_is_tizen_content(bool is_tizen_content) {
    is_tizen_content_ = is_tizen_content;
  }

 private:
  std::string src_;
  std::string type_;
  std::string encoding_;
  bool is_tizen_content_;
};

class ContentHandler : public parser::ManifestHandler {
 public:
  enum class ParseResult {
    OK,
    IGNORE,
    ERROR,
  };

  ContentHandler();
  virtual ~ContentHandler();

  ParseResult ParseAndSetContentValue(
      const parser::DictionaryValue& dict,
      std::shared_ptr<wgt::parse::ContentInfo>* content,
      std::string* error);

  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  std::string Key() const override;

 private:
  bool w3c_content_found_;
  bool tizen_content_found_;
};

}  // namespace parse
}  // namespace wgt

#endif  // MANIFEST_HANDLERS_CONTENT_HANDLER_H_
