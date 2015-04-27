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
  void set_src(const std::string& src) { src_ = src; }
  bool is_tizen_content() const {
    return is_tizen_content_;
  }
  void set_is_tizen_content(bool is_tizen_content) {
    is_tizen_content_ = is_tizen_content;
  }

 private:
  std::string src_;
  bool is_tizen_content_;
};

class ContentHandler : public parser::ManifestHandler {
 public:
  ContentHandler();
  virtual ~ContentHandler();
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  std::string Key() const override;
};

}  // namespace parse
}  // namespace wgt

#endif  // MANIFEST_HANDLERS_CONTENT_HANDLER_H_
