// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/author_handler.h"

#include <cassert>
#include <map>
#include <regex>
#include <utility>

#include "manifest_parser/manifest_util.h"
#include "manifest_parser/utils/iri_util.h"
#include "manifest_parser/utils/logging.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {
namespace parse {

namespace keys = tpk::application_keys;

namespace {

const char kAuthorKey[] = "manifest.author";
const char kAuthorKeyText[] = "#text";
const char kAuthorEmailKey[] = "@email";
const char kAuthorHrefKey[] = "@href";

void ParseAuthorAndStore(
    const parser::DictionaryValue& author_dict,
    AuthorInfo* author) {
  std::string email;
  author_dict.GetString(kAuthorEmailKey, &email);
  author->set_email(email);

  std::string href;
  author_dict.GetString(kAuthorHrefKey, &href);
  author->set_href(href);

  std::string name;
  author_dict.GetString(kAuthorKeyText, &name);
  author->set_name(name);
}

}  // namespace

bool AuthorHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<AuthorInfo> author(new AuthorInfo());
  parser::Value* value = nullptr;
  if (!manifest.Get(kAuthorKey, &value)) {
    *error = "Author element not found";
    return true;
  }

  if (value->GetType() == parser::Value::TYPE_DICTIONARY) {
    const parser::DictionaryValue* dict;
    value->GetAsDictionary(&dict);
    ParseAuthorAndStore(*dict, author.get());
  } else {
    *error = "Cannot parse author element";
    return false;
  }

  *output = std::static_pointer_cast<parser::ManifestData>(author);
  return true;
}

bool AuthorHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const AuthorInfo& author =
      static_cast<const AuthorInfo&>(data);

  if (author.email().empty()) {
    *error = "The email child element of author element is obligatory";
    return false;
  }
  if (!parser::ValidateEmailAddress(author.email())) {
    *error = "The author email address is not valid";
    return false;
  }


  const std::string& href = author.href();
  if (href.empty()) {
    *error =
        "The href child element of author element is obligatory";
    return false;
  }

  if (!parser::utils::IsValidIRI(href)) {
    *error =
        "The href child element of author element is not valid url";
    return false;
  }

  const std::string& name = author.name();
  if (name.empty()) {
    *error =
        "The name child element of author element is obligatory";
    return false;
  }
  return true;
}

std::string AuthorInfo::Key() {
  return kAuthorKey;
}

std::string AuthorHandler::Key() const {
  return kAuthorKey;
}

}   // namespace parse
}   // namespace tpk
