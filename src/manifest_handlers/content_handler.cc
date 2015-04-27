// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_handlers/content_handler.h"
#include "utils/logging.h"

namespace keys = wgt::application_widget_keys;

namespace {

void ParseAndUpdateContentValue(const parser::DictionaryValue& dict,
    std::shared_ptr<wgt::parse::ContentInfo> content) {
  std::string src;
  std::string element_namespace;
  if (!dict.GetString(keys::kTizenContentSrcKey, &src))
    return;
  dict.GetString(keys::kNamespaceKey, &element_namespace);
  // tizen:content already found, ignore the rest
  if (content->is_tizen_content())
    return;

  // if empty or not defined then skip it
  if (src.empty())
    return;

  // set new content if not found yet or tizen:content found after content tag
  if (content->src().empty()) {
    content->set_src(src);
    content->set_is_tizen_content(\
        element_namespace == keys::kTizenNamespacePrefix);
  } else if (element_namespace == keys::kTizenNamespacePrefix &&
             !content->is_tizen_content()) {
    content->set_src(src);
    content->set_is_tizen_content(true);
  }
}

}  // namespace

namespace wgt {
namespace parse {

ContentHandler::ContentHandler() {
}

ContentHandler::~ContentHandler() {
}

bool ContentHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* /*error*/) {
  std::shared_ptr<ContentInfo> content_info(new ContentInfo);
  parser::Value* value = nullptr;
  manifest.Get(keys::kTizenContentKey, &value);

  if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    ParseAndUpdateContentValue(*dict, content_info);
  } else if (value->GetType() == parser::Value::TYPE_LIST) {
    parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      if (item->GetAsDictionary(&dict))
        ParseAndUpdateContentValue(*dict, content_info);
    }
  } else {
    return true;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(content_info);
  return true;
}

std::string ContentHandler::Key() const {
  return keys::kTizenContentKey;
}

}  // namespace parse
}  // namespace wgt
