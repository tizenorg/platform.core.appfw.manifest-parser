// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/widget_handler.h"

#include <string.h>
#include <iri.h>

#include <cassert>
#include <map>
#include <utility>
#include <set>

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_parser/manifest_constants.h"
#include "manifest_parser/values.h"
#include "utils/logging.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

namespace {
// Below key names are readable from Javascript widget interface.
const char kAuthor[] = "author";
const char kDecription[] = "description";
const char kName[] = "name";
const char kShortName[] = "shortName";
const char kVersion[] = "version";
const char kID[] = "id";
const char kAuthorEmail[] = "authorEmail";
const char kAuthorHref[] = "authorHref";
const char kHeight[] = "height";
const char kWidth[] = "width";
const char kPreferences[] = "preferences";
const char kDefaultLocale[] = "defaultlocale";
const char kViewModes[] = "viewmodes";

// Child keys inside 'preferences' key.
const char kPreferencesName[] = "@name";
const char kPreferencesValue[] = "@value";
const char kPreferencesReadonly[] = "@readonly";

bool ParserPreferenceItem(const parser::Value* val,
                          Preference** output,
                          std::string* error) {
  const parser::DictionaryValue* pref_dict;
  if (!val->GetAsDictionary(&pref_dict)) {
    *error = "Parsing preference element failed";
    return false;
  }
  std::string name, value;
  bool readonly;
  pref_dict->GetString(kPreferencesName, &name);
  pref_dict->GetString(kPreferencesValue, &value);
  pref_dict->GetBoolean(kPreferencesReadonly, &readonly);
  *output = new Preference(name, value, readonly);
  return true;
}

}  // namespace

WidgetInfo::WidgetInfo() {}

WidgetInfo::~WidgetInfo() {}

void WidgetInfo::AddName(const std::string& locale, const std::string& name) {
  if (name_set_.find(locale) != name_set_.end())
    return;
  name_set_.insert(std::make_pair(locale, name));
}

void WidgetInfo::AddShortName(const std::string& locale,
                              const std::string& short_name) {
  if (short_name_set_.find(locale) != short_name_set_.end())
    return;
  short_name_set_.insert(std::make_pair(locale, short_name));
}

void WidgetInfo::AddDescription(const std::string& locale,
                    const std::string& description) {
  if (description_set_.find(locale) != description_set_.end())
    return;
  description_set_.insert(std::make_pair(locale, description));
}

const std::map<std::string, std::string>& WidgetInfo::name_set() const {
  return name_set_;
}

const std::map<std::string, std::string>& WidgetInfo::short_name_set() const {
  return short_name_set_;
}

const std::map<std::string, std::string>& WidgetInfo::description_set() const {
  return description_set_;
}

const std::string& WidgetInfo::id() const {
  return id_;
}

const std::string& WidgetInfo::version() const {
  return version_;
}

const std::string& WidgetInfo::view_modes() const {
  return viewmodes_;
}

const std::string& WidgetInfo::default_locale() const {
  return default_locale_;
}

const std::string& WidgetInfo::author() const {
  return author_;
}

const std::string& WidgetInfo::author_email() const {
  return author_email_;
}

const std::string& WidgetInfo::author_href() const {
  return author_href_;
}

const std::string& WidgetInfo::widget_namespace() const {
  return widget_namespace_;
}

unsigned int WidgetInfo::height() const {
  return height_;
}

unsigned int WidgetInfo::width() const {
  return width_;
}

const std::vector<Preference*>& WidgetInfo::preferences() const {
  return preferences_;
}

WidgetHandler::WidgetHandler() {}

WidgetHandler::~WidgetHandler() {}

void WidgetHandler::ParseSingleLocalizedDescriptionElement(
    const parser::DictionaryValue* item_dict, const std::string& parent_lang,
    std::shared_ptr<WidgetInfo> info) {
  bool lang_overwriten = false;
  std::string lang;
  std::string text;

  if (item_dict->HasKey(keys::kXmlLangKey)) {
    lang_overwriten = true;
    item_dict->GetString(keys::kXmlLangKey, &lang);
  }
  item_dict->GetString(parser::kXmlTextKey, &text);
  // TODO(t.iwanek): check internationalization tag validity...
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
      ParseSingleLocalizedDescriptionElement(dict, parent_lang, info);
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          ParseSingleLocalizedDescriptionElement(dict, parent_lang,
              info);
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
  }
  if (item_dict->HasKey(keys::kShortKey)) {
    item_dict->GetString(keys::kShortKey, &short_name);
  }
  item_dict->GetString(parser::kXmlTextKey, &name);

  // ignore if given language already spotted
  if (info->name_set_.find(lang) != info->name_set_.end())
    return;

  // TODO(t.iwanek): check internationalization tag validity...

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
      ParseSingleLocalizedNameElement(dict, parent_lang, info);
    } else if (val->GetAsList(&list)) {
      for (auto& item : *list)
        if (item->GetAsDictionary(&dict))
          ParseSingleLocalizedNameElement(dict, parent_lang, info);
    }
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
  if (manifest.HasPath(keys::kAuthorKey))
    manifest.GetString(keys::kAuthorKey, &widget_info->author_);

  ParseLocalizedDescriptionElements(manifest, parent_lang, widget_info);
  ParseLocalizedNameElements(manifest, parent_lang, widget_info);

  if (manifest.HasPath(keys::kVersionKey))
    manifest.GetString(keys::kVersionKey, &widget_info->version_);
  if (manifest.HasPath(keys::kIDKey)) {
    std::string id;
    manifest.GetString(keys::kIDKey, &id);
    if (!id.empty() && ValidateIRIType(id))
      widget_info->id_ = id;
  }
  if (manifest.HasPath(keys::kAuthorEmailKey))
    manifest.GetString(keys::kAuthorEmailKey, &widget_info->author_email_);
  if (manifest.HasPath(keys::kAuthorHrefKey)) {
    std::string author_href;
    manifest.GetString(keys::kAuthorHrefKey, &author_href);
    if (!author_href.empty() && ValidateIRIType(author_href))
      widget_info->author_href_ = author_href;
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
    manifest.GetString(keys::kDefaultLocaleKey, &widget_info->viewmodes_);

  if (manifest.HasPath(keys::kPreferencesKey)) {
    const parser::Value* val = nullptr;
    if (manifest.Get(keys::kPreferencesKey, &val)) {
      if (val->GetType() == parser::Value::TYPE_LIST) {
        // list of preferences
        const parser::ListValue* pref_list;
        if (val->GetAsList(&pref_list)) {
          // get all preferences
          for (const auto& pref : *pref_list) {
            Preference* preference;
            if (!ParserPreferenceItem(pref, &preference, error))
              return false;
            widget_info->preferences_.push_back(preference);
          }
        } else {
          *error = "Preference list exists in manifest, but is not accessible.";
          return false;
        }
      } else if (val->GetType() == parser::Value::TYPE_DICTIONARY) {
        // only one preference
        Preference* pref;
        if (!ParserPreferenceItem(val, &pref, error))
          return false;
        widget_info->preferences_.push_back(pref);
      }
    }
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

bool WidgetHandler::AlwaysParseForType() const {
  return true;
}

std::string WidgetHandler::Key() const {
  return application_widget_keys::kTizenWidgetKey;
}

bool WidgetHandler::ValidateIRIType(const std::string& prop) const {
  std::unique_ptr<iri_struct, decltype(&iri_destroy)> iri(
    iri_parse(prop.c_str()), iri_destroy);

  return
    iri != NULL &&
    iri->scheme != NULL && (
      iri->display != NULL ||
      iri->user != NULL ||
      iri->auth != NULL ||
      iri->password != NULL ||
      iri->host != NULL ||
      iri->path != NULL ||
      iri->query != NULL ||
      iri->anchor != NULL ||
      iri->qparams != NULL ||
      iri->schemelist != NULL );
}

}  // namespace parse
}  // namespace wgt
