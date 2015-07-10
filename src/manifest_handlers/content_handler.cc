// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/content_handler.h"

#include <gmime-2.6/gmime/gmime.h>

#include <map>
#include <set>

#include "manifest_handlers/application_manifest_constants.h"
#include "utils/iri_util.h"
#include "utils/logging.h"
#include "utils/string_util.h"

namespace keys = wgt::application_widget_keys;

namespace {

const char kMimeCharsetComponent[] = "charset";
const char kDefaultMimeType[] = "text/html";
const char kDefaultEncoding[] = "UTF-8";

const std::set<std::string> ValidMimeTypeStartFile = {
  "text/html",
  "application/xhtml+xml",
  "image/svg+xml"
};

std::pair<std::string, std::string> GetMediaTypeAndCharset(
    const std::string& type) {
  GMimeContentType* content_type =
      g_mime_content_type_new_from_string(type.c_str());
  if (!content_type)
    return nullptr;
  std::string mime;
  const char* mime_str = g_mime_content_type_get_media_type(content_type);
  if (mime_str)
    mime = mime_str;
  std::string charset;
  const char* charset_str = g_mime_content_type_get_parameter(content_type,
                              kMimeCharsetComponent);
  if (charset_str)
    charset = charset_str;
  gfree(content_type);
  return std::make_pair(mime, charset);
}

bool ValidateMimeTypeStartFile(const std::string& type) {
  return ValidMimeTypeStartFile.find(
        parser::utils::CollapseWhitespaceUTF8(type)) !=
      ValidMimeTypeStartFile.end();
}

}  // namespace

namespace wgt {
namespace parse {

ContentHandler::ContentHandler()
    : w3c_content_found_(false),
      tizen_content_found_(false) {
}

ContentHandler::~ContentHandler() {
}

/**
 * @brief ParseAndSetContentValue
 *
 * This function should return parse result and set output parameter content
 * to valid content if returning OK
 *
 * @param dict input dictionary for content element
 * @param content content element to be set if valid one found
 * @param error parser error to be set if parsing failed with error
 * @return enum value describing result of parsing
 */
ContentHandler::ParseResult ContentHandler::ParseAndSetContentValue(
    const parser::DictionaryValue& dict,
    std::shared_ptr<wgt::parse::ContentInfo>* content,
    std::string* error) {
  std::string element_namespace;
  dict.GetString(keys::kNamespaceKey, &element_namespace);

  if (element_namespace == keys::kTizenNamespacePrefix) {
    if (tizen_content_found_) {
      // tizen:content already found
      return ParseResult::IGNORE;
    }
    tizen_content_found_ = true;
  } else {
    if (w3c_content_found_) {
      // ignore all content element that are not first
      return ParseResult::IGNORE;
    }
    w3c_content_found_ = true;
  }

  std::string src;
  if (!dict.GetString(keys::kTizenContentSrcKey, &src)) {
    return ParseResult::IGNORE;
  }

  // error if empty
  if (src.empty()) {
    // ignore this element
    return ParseResult::IGNORE;
  }

  std::string type = kDefaultMimeType;
  dict.GetString(keys::kTizenContentTypeKey, &type);
  // TODO(t.iwanek): this will fail for "quoted-string"
  //                 use/implement proper mime parsing...
  std::string mime;
  std::string charset;
  std::tie(mime, charset) = GetMediaTypeAndCharset(type);

  if (!mime.empty()) {
    if (!ValidateMimeTypeStartFile(mime)) {
      *error = "Not proper type of starting file";
      return ParseResult::IGNORE;
    }
  }

  std::string encoding = kDefaultEncoding;
  if (!dict.GetString(keys::kTizenContentEncodingKey, &encoding)) {
    if (!charset.empty()) {
      encoding = charset;
    }
  }

  if (*content && (*content)->is_tizen_content()) {
    // Prefer tizen:content if both are correct
    return ParseResult::IGNORE;
  }

  content->reset(new wgt::parse::ContentInfo());
  (*content)->set_src(src);
  (*content)->set_type(type);
  (*content)->set_encoding(encoding);
  (*content)->set_is_tizen_content(
      element_namespace == keys::kTizenNamespacePrefix);
  return ParseResult::OK;
}

bool ContentHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<ContentInfo> content_info;
  parser::Value* value = nullptr;
  manifest.Get(keys::kTizenContentKey, &value);

  w3c_content_found_ = false;
  tizen_content_found_ = false;
  if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict = nullptr;
    value->GetAsDictionary(&dict);
    if (ParseAndSetContentValue(*dict, &content_info, error)
        == ParseResult::ERROR) {
      return false;
    }
  } else if (value->GetType() == parser::Value::TYPE_LIST) {
    parser::ListValue* list = nullptr;
    value->GetAsList(&list);
    // Iterate through contents and set values
    for (auto& item : *list) {
      const parser::DictionaryValue* dict = nullptr;
      if (item->GetAsDictionary(&dict)) {
        if (ParseAndSetContentValue(*dict, &content_info, error)
            == ParseResult::ERROR) {
          return false;
        }
      }
    }
  } else {
    return true;
  }

  if (content_info)
    *output = std::static_pointer_cast<parser::ManifestData>(content_info);
  return true;
}

std::string ContentHandler::Key() const {
  return keys::kTizenContentKey;
}

}  // namespace parse
}  // namespace wgt
