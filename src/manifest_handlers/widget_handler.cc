// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/widget_handler.h"

#include <string.h>

#include <algorithm>
#include <cassert>
#include <map>
#include <utility>
#include <set>

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_parser/values.h"
#include "utils/iri_util.h"
#include "utils/language_tag_validator.h"
#include "utils/logging.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

namespace {

bool ParserPreferenceItem(const parser::Value* val,
                          Preference** output,
                          std::string* error) {
  const parser::DictionaryValue* pref_dict;
  if (!val->GetAsDictionary(&pref_dict)) {
    *error = "Parsing preference element failed";
    return false;
  }
  std::string name;
  std::string value;
  std::string readonly = "false";
  pref_dict->GetString(keys::kPreferencesNameKey, &name);
  pref_dict->GetString(keys::kPreferencesValueKey, &value);
  pref_dict->GetString(keys::kPreferencesReadonlyKey, &readonly);
  *output = new Preference(name, value, readonly == "true");
  return true;
}

}  // namespace

void WidgetHandler::ParseSingleLocalizedLicenseElement(
    const parser::DictionaryValue* item_dict, const std::string& parent_lang,
    std::shared_ptr<WidgetInfo> info) {
  bool lang_overwriten = false;
  std::string lang;
  std::string text;
  std::string href;

  if (item_dict->HasKey(keys::kXmlLangKey)) {
    lang_overwriten = true;
    item_dict->GetString(keys::kXmlLangKey, &lang);
    if (!utils::w3c_languages::ValidateLanguageTag(lang)) {
      LOG(ERROR) << "Tag " << lang << " is invalid";
      return;
    }
  }
  if (item_dict->HasKey(keys::kXmlHrefKey)) {
    item_dict->GetString(keys::kXmlHrefKey, &href);
  }
  item_dict->GetString(keys::kXmlTextKey, &text);
  // TODO(w.kosowicz) check where href should be put...
  if (lang_overwriten) {
    info->license_set_.insert(std::make_pair(lang, text + href));
  } else {
    info->license_set_.insert(std::make_pair(parent_lang, text + href));
  }
}

void WidgetHandler::ParseLocalizedLicenseElements(
    const parser::Manifest& manifest,
    const std::string& parent_lang,
    std::shared_ptr<WidgetInfo> info) {
  if (!manifest.HasPath(keys::kLicenseKey))
    return;

  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  if (manifest.Get(keys::kLicenseKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (parser::VerifyElementNamespace(*dict, keys::kWidgetNamespacePrefix))
        ParseSingleLocalizedLicenseElement(dict, parent_lang, info);
    } else if (val->GetAsList(&list)) {
      for_each(list->begin(), list->end(), [list, &dict,
               parent_lang, info, this](parser::Value* item) {
        if (item->GetAsDictionary(&dict))
          if (parser::VerifyElementNamespace(*dict,
                                             keys::kWidgetNamespacePrefix))
            ParseSingleLocalizedLicenseElement(dict, parent_lang, info);
      });
    }
  }
}

void WidgetHandler::ParseSingleLocalizedDescriptionElement(
    const parser::DictionaryValue* item_dict, const std::string& parent_lang,
    std::shared_ptr<WidgetInfo> info) {
  bool lang_overwriten = false;
  std::string lang;
  std::string text;

  if (item_dict->HasKey(keys::kXmlLangKey)) {
    lang_overwriten = true;
    item_dict->GetString(keys::kXmlLangKey, &lang);
    if (!utils::w3c_languages::ValidateLanguageTag(lang)) {
      LOG(ERROR) << "Tag " << lang << " is invalid";
      return;
    }
  }
  item_dict->GetString(keys::kXmlTextKey, &text);
  if (lang_overwriten) {
    info->description_set_.insert(std::make_pair(lang, text));
  } else {
    info->description_set_.insert(std::make_pair(parent_lang, text));
  }
}

void WidgetHandler::ParseLocalizedDescriptionElements(
    const parser::Manifest& manifest,
    const std::string& parent_lang,
    std::shared_ptr<WidgetInfo> info) {
  if (!manifest.HasPath(keys::kDescriptionKey))
    return;

  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  if (manifest.Get(keys::kDescriptionKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (parser::VerifyElementNamespace(*dict, keys::kWidgetNamespacePrefix))
        ParseSingleLocalizedDescriptionElement(dict, parent_lang, info);
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          if (parser::VerifyElementNamespace(*dict,
                                             keys::kWidgetNamespacePrefix))
            ParseSingleLocalizedDescriptionElement(dict, parent_lang, info);
    }
  }
}

void WidgetHandler::ParseSingleLocalizedNameElement(
    const parser::DictionaryValue* item_dict, const std::string& parent_lang,
    std::shared_ptr<WidgetInfo> info) {
  bool lang_overwriten = false;
  std::string lang;
  std::string name;
  std::string short_name;

  if (item_dict->HasKey(keys::kXmlLangKey)) {
    lang_overwriten = true;
    item_dict->GetString(keys::kXmlLangKey, &lang);
    if (!utils::w3c_languages::ValidateLanguageTag(lang)) {
      LOG(ERROR) << "Tag " << lang << " is invalid";
      return;
    }
  }
  if (item_dict->HasKey(keys::kShortKey)) {
    item_dict->GetString(keys::kShortKey, &short_name);
  }
  item_dict->GetString(keys::kXmlTextKey, &name);

  // ignore if given language already spotted
  if (info->name_set_.find(lang) != info->name_set_.end())
    return;

  if (lang_overwriten) {
    info->name_set_.insert(std::make_pair(lang, name));
    if (!short_name.empty())
      info->short_name_set_.insert(std::make_pair(lang, short_name));
  } else {
    info->name_set_.insert(std::make_pair(parent_lang, name));
    if (!short_name.empty())
      info->short_name_set_.insert(std::make_pair(parent_lang, short_name));
  }
}

void WidgetHandler::ParseLocalizedNameElements(const parser::Manifest& manifest,
    const std::string& parent_lang, std::shared_ptr<WidgetInfo> info) {
  if (!manifest.HasPath(keys::kNameKey))
    return;

  const parser::Value* val = nullptr;
  const parser::DictionaryValue* dict = nullptr;
  const parser::ListValue* list = nullptr;
  if (manifest.Get(keys::kNameKey, &val)) {
    if (val->GetAsDictionary(&dict)) {
      if (parser::VerifyElementNamespace(*dict, keys::kWidgetNamespacePrefix))
        ParseSingleLocalizedNameElement(dict, parent_lang, info);
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          if (parser::VerifyElementNamespace(*dict,
                                             keys::kWidgetNamespacePrefix))
            ParseSingleLocalizedNameElement(dict, parent_lang, info);
    }
  }
}

void WidgetHandler::ParseSingleAuthorElement(
    const parser::DictionaryValue* author_dict,
    std::shared_ptr<WidgetInfo> info) {
  author_dict->GetString(keys::kXmlTextKey, &info->author_);
  author_dict->GetString(keys::kAuthorEmailKey, &info->author_email_);
  std::string author_href;
  author_dict->GetString(keys::kAuthorHrefKey, &author_href);
  if (!author_href.empty() && parser::utils::IsValidIRI(author_href))
    info->author_href_ = author_href;
}

void WidgetHandler::ParseAuthorElements(
    const parser::Manifest& manifest,
    std::shared_ptr<WidgetInfo> info) {
  if (manifest.HasPath(keys::kAuthorKey)) {
    const parser::Value* author_value = nullptr;
    manifest.Get(keys::kAuthorKey, &author_value);
    auto& authors = parser::GetOneOrMany(manifest.value(), keys::kAuthorKey,
                                         keys::kWidgetNamespacePrefix);
    if (!authors.empty())
      ParseSingleAuthorElement(authors[0], info);
  }
}

bool WidgetHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<WidgetInfo> widget_info(new WidgetInfo());
  widget_info->preferences_ = std::vector<Preference*>();

  std::string parent_lang;
  if (manifest.HasPath(keys::kWidgetNamespaceKey)) {
    manifest.GetString(keys::kWidgetNamespaceKey,
                       &widget_info->widget_namespace_);
    manifest.GetString(keys::kWidgetLangKey,
                       &parent_lang);
  }

  if (widget_info->widget_namespace_ != keys::kWidgetNamespacePrefix) {
    *error = "Wrong namespace of <widget> element. Config.xml is invalid";
    return false;
  }

  ParseAuthorElements(manifest, widget_info);

  ParseLocalizedDescriptionElements(manifest, parent_lang, widget_info);
  ParseLocalizedNameElements(manifest, parent_lang, widget_info);
  ParseLocalizedLicenseElements(manifest, parent_lang, widget_info);

  if (manifest.HasPath(keys::kVersionKey))
    manifest.GetString(keys::kVersionKey, &widget_info->version_);
  if (manifest.HasPath(keys::kIDKey)) {
    std::string id;
    manifest.GetString(keys::kIDKey, &id);
    if (!id.empty() && parser::utils::IsValidIRI(id))
      widget_info->id_ = id;
  }
  if (manifest.HasPath(keys::kHeightKey)) {
    int h;
    manifest.GetInteger(keys::kHeightKey, &h);
    widget_info->height_ = static_cast<unsigned int>(h);
  }
  if (manifest.HasPath(keys::kWidthKey)) {
    int w;
    manifest.GetInteger(keys::kWidthKey, &w);
    widget_info->width_ = static_cast<unsigned int>(w);
  }
  if (manifest.HasPath(keys::kDefaultLocaleKey))
    manifest.GetString(keys::kDefaultLocaleKey, &widget_info->default_locale_);
  if (manifest.HasPath(keys::kViewModesKey))
    manifest.GetString(keys::kViewModesKey, &widget_info->viewmodes_);

  for (auto& pref_dict : parser::GetOneOrMany(manifest.value(),
                                              keys::kPreferencesKey,
                                              keys::kWidgetNamespacePrefix)) {
    Preference* preference = nullptr;
    if (!ParserPreferenceItem(pref_dict, &preference, error))
      return false;
    widget_info->preferences_.push_back(preference);
  }

  *output = std::static_pointer_cast<parser::ManifestData>(widget_info);
  return true;
}

bool WidgetHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const WidgetInfo& widget_info = static_cast<const WidgetInfo&>(data);
  if (widget_info.widget_namespace() != keys::kWidgetNamespacePrefix) {
    *error = "The widget namespace is invalid.";
    return false;
  }
  return true;
}

std::string WidgetHandler::Key() const {
  return keys::kTizenWidgetKey;
}

}  // namespace parse
}  // namespace wgt
