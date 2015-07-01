// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/content_handler.h"

#include <set>

#include "manifest_handlers/application_manifest_constants.h"
#include "utils/iri_util.h"
#include "utils/logging.h"
#include "utils/string_util.h"

namespace keys = wgt::application_widget_keys;

namespace {
const std::set<std::string> ValidMimeTypeStartFile = {
  "text/html",
  "application/xhtml+xml",
  "image/svg+xml"
};

bool ValidateMimeTypeStartFile(const std::string& type) {
  return ValidMimeTypeStartFile.find(
        parser::utils::CollapseWhitespaceUTF8(type)) !=
      ValidMimeTypeStartFile.end();
}

bool ParseAndUpdateContentValue(const parser::DictionaryValue& dict,
    std::shared_ptr<wgt::parse::ContentInfo> content, bool* w3c_content_found,
    std::string* error) {
  std::string src;
  std::string type;
  // src is mandatory
  if (!dict.GetString(keys::kTizenContentSrcKey, &src)) {
    *error = "<content> / <tizen:content> tags requires src attribute";
    return false;
  }

  if (dict.GetString(keys::kTizenContentTypeKey, &type) &&
      !ValidateMimeTypeStartFile(type)) {
      *error = "Not proper type of starting file";
      return false;
  }

  std::string encoding;
  // default encoding setting
  if (!dict.GetString(keys::kTizenContentEncodingKey, &encoding))
    encoding = "UTF-8";

  std::string element_namespace;
  dict.GetString(keys::kNamespaceKey, &element_namespace);

  // error if empty
  if (src.empty()) {
    // ignore this element
    return true;
  }

  if (element_namespace == keys::kTizenNamespacePrefix) {
    if (!parser::utils::IsValidIRI(src)) {
      *error = "src of <tizen:content> should be valid external url";
      return false;
    }

    if (!content->is_tizen_content()) {
      // override normal content
      content->set_src(src);
      content->set_encoding(encoding);
      content->set_is_tizen_content(true);
    }
  } else {
    if (!*w3c_content_found && !content->is_tizen_content()) {
      content->set_src(src);
      content->set_encoding(encoding);
      content->set_is_tizen_content(false);
    }
    *w3c_content_found = true;
  }
  return true;
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
    std::string* error) {
  std::shared_ptr<ContentInfo> content_info(new ContentInfo);
  parser::Value* value = nullptr;
  manifest.Get(keys::kTizenContentKey, &value);

  bool w3c_content_found = false;
  if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    if (!ParseAndUpdateContentValue(*dict, content_info, &w3c_content_found,
                                    error)) {
      return false;
    }
  } else if (value->GetType() == parser::Value::TYPE_LIST) {
    parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      if (item->GetAsDictionary(&dict)) {
        if (!ParseAndUpdateContentValue(*dict, content_info,
                                        &w3c_content_found, error)) {
          return false;
        }
      }
    }
  } else {
    return true;
  }

  // found w3c or tizen content
  if (w3c_content_found || content_info->is_tizen_content())
    *output = std::static_pointer_cast<parser::ManifestData>(content_info);
  return true;
}

std::string ContentHandler::Key() const {
  return keys::kTizenContentKey;
}

}  // namespace parse
}  // namespace wgt
