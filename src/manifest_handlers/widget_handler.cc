// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/widget_handler.h"

#include <string.h>

#include <cassert>
#include <map>
#include <utility>
#include <set>

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_parser/manifest_constants.h"
#include "manifest_parser/values.h"

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
                          Preference* output,
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
  output = new Preference(name, value, readonly);
  return true;
}

}  // namespace

WidgetInfo::WidgetInfo() {}

WidgetInfo::~WidgetInfo() {}

void WidgetInfo::set_name(const std::string& name) {
  name_ = name;
}

void WidgetInfo::set_short_name(const std::string& short_name) {
  short_name_ = short_name;
}

void WidgetInfo::set_description(const std::string& description) {
  description_ = description_;
}

const std::string& WidgetInfo::name() const {
  return name_;
}

const std::string& WidgetInfo::short_name() const {
  return short_name_;
}

const std::string& WidgetInfo::description() const {
  return description_;
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

bool WidgetHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<WidgetInfo> widget_info(new WidgetInfo());
  widget_info->preferences_ = std::vector<Preference*>();

  if (manifest.HasPath(keys::kWidgetNamespaceKey))
    manifest.GetString(keys::kWidgetNamespaceKey,
                       &widget_info->widget_namespace_);
  if (manifest.HasPath(keys::kAuthorKey))
    manifest.GetString(keys::kAuthorKey, &widget_info->author_);
  if (manifest.HasPath(keys::kDescriptionKey))
    manifest.GetString(keys::kDescriptionKey, &widget_info->description_);
  if (manifest.HasPath(keys::kNameKey))
    manifest.GetString(keys::kNameKey, &widget_info->name_);
  if (manifest.HasPath(keys::kShortNameKey))
    manifest.GetString(keys::kShortNameKey, &widget_info->short_name_);
  if (manifest.HasPath(keys::kVersionKey))
    manifest.GetString(keys::kVersionKey, &widget_info->version_);
  if (manifest.HasPath(keys::kIDKey))
    manifest.GetString(keys::kIDKey, &widget_info->id_);
  if (manifest.HasPath(keys::kAuthorEmailKey))
    manifest.GetString(keys::kAuthorEmailKey, &widget_info->author_email_);
  if (manifest.HasPath(keys::kAuthorHrefKey))
    manifest.GetString(keys::kAuthorHrefKey, &widget_info->author_href_);
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
            if (!ParserPreferenceItem(pref, preference, error))
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
        if (!ParserPreferenceItem(val, pref, error))
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

}  // namespace parse
}  // namespace wgt
