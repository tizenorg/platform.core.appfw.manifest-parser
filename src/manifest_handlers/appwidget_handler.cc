// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/appwidget_handler.h"

#include <cassert>
#include <cstdlib>
#include <limits>
#include <regex>
#include <set>

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_handlers/tizen_application_handler.h"
#include "manifest_parser/values.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

namespace {

const char kErrMsgInvalidDictionary[] =
    "Cannot get key value as a dictionary. Key name: ";
const char kErrMsgInvalidList[] =
    "Cannot get key value as a list. Key name: ";
const char kErrMsgNoMandatoryKey[] =
    "Cannot find mandatory key. Key name: ";
const char kErrMsgInvalidKeyValue[] =
    "Invalid key value. Key name: ";
const char kErrMsgMultipleKeys[] =
    "Too many keys found. Key name: ";
const char kErrMsgNoNamespace[] =
    "Element pointed by key has no namespace specified. Key name: ";
const char kErrMsgInvalidNamespace[] =
    "Invalid namespace of element pointed by key. Key name: ";
const char kErrMsgAppWidgetInfoNotFound[] =
    "Cannot access app-widget info object.";
const char kErrMsgApplicationInfoNotFound[] =
    "Cannot access application info object.";
const char kErrMsgDuplicatedAppWidgetId[] =
    "Duplicated value of an id attribute in app-widget element. The value: ";
const char kErrMsgInvalidAppWidgetIdBeginning[] =
    "Invalid beginning of an id attribute value in app-widget element."
    " The value: ";
const char kErrMsgInvalidAppWidgetIdFormat[] =
    "Invalid format of an id attribute value in app-widget element."
    " The value: ";
const char kErrMsgUpdatePeriodOutOfDomain[] =
    "Value of an update-period attribute in app-widget element out of domain."
    " The value: ";
const char kErrMsgNoLabel[] =
    "No box-label element in app-widget element.";
const char kErrMsgInvalidIconSrc[] =
    "Invalid path in a src attribute of box-icon element. The value: ";
const char kErrMsgInvalidContentSrc[] =
    "Invalid path or url in a src attribute of box-content element."
    " The value: ";
const char kErrMsgInvalidContentSizePreview[] =
    "Invalid path in a preview attribute of box-size element. The value: ";
const char kErrMsgNoMandatoryContentSize1x1[] =
    "No mandatory box-size element (1x1) in box-content element.";
const char kErrMsgInvalidContentDropViewSrc[] =
    "Invalid path or url in a src attribute of pd element. The value: ";
const char kErrMsgContentDropViewHeightOutOfDomain[] =
    "Value of a height attribute in box-content element out of domain."
    " The value: ";

const std::regex kStringRegex("[.][0-9a-zA-Z]+");

// If the error parameter is specified, it is filled with the given message
// otherwise it does nothing.
void SetError(const std::string& message,
    std::string* error) {
  if (error)
    *error = message;
}

// If the error parameter is specified, it is filled with concatenation
// of message and arg parameters otherwise it does nothing.
void SetError(const std::string& message,
    const std::string& arg, std::string* error) {
  if (error)
    *error = message + arg;
}

// Retrieves a mandatory dictionary from specified manifest and specified key.
// Returns true, if the ditionary is found or false otherwise. If the error
// parameter is specified, it is also filled with proper message.
bool GetMandatoryDictionary(const parser::Manifest& manifest,
    const std::string& key, const parser::DictionaryValue** dict,
    std::string* error) {
  assert(dict);
  if (!manifest.HasPath(key)) {
    SetError(kErrMsgNoMandatoryKey, key, error);
    return false;
  }
  if (!manifest.GetDictionary(key, dict) || !*dict) {
    SetError(kErrMsgInvalidDictionary, key, error);
    return false;
  }
  return true;
}

// Converts given text value to a value of specific type. Returns true
// if convertion is successful or false otherwise.
template <typename ValueType>
bool ConvertValue(const std::string& /*str_value*/, ValueType* /*value*/) {
  assert(false && "Use one of already defined template specializations"
                  " or define a new one.");
  return false;
}

// Converts given text value to a string value. Returns true
// if convertion is successful or false otherwise.
template <>
bool ConvertValue(const std::string& str_value, std::string* value) {
  assert(value);
  *value = str_value;
  return true;
}

// Converts given text value to a boolean value. Returns true
// if convertion is successful or false otherwise.
template <>
bool ConvertValue(const std::string& str_value, bool* value) {
  assert(value);
  if (str_value == "true") {
    *value = true;
    return true;
  }
  if (str_value == "false") {
    *value = false;
    return true;
  }
  return false;
}

// Converts given text value to an integer value. Returns true
// if convertion is successful or false otherwise.
template <>
bool ConvertValue(const std::string& str_value, int* value) {
  assert(value);
  char* end = nullptr;
  *value = strtol(str_value.c_str(), &end, 10);
  return end == &*str_value.end();
}

// Converts given text value to a floating point value. Returns true
// if convertion is successful or false otherwise.
template <>
bool ConvertValue(const std::string& str_value, double* value) {
  assert(value);
  char* end = nullptr;
  *value = strtod(str_value.c_str(), &end);
  return end == &*str_value.end();
}

// Retrieves a mandatory value from specified dictionary and specified key.
// Returns true, if the value is found or false otherwise. If the error
// parameter is specified, it is also filled with proper message.
template <typename ValueType>
bool GetMandatoryValue(const parser::DictionaryValue& dict,
    const std::string& key, ValueType* value, std::string* error) {
  assert(value);
  std::string tmp;
  if (!dict.GetString(key, &tmp)) {
    SetError(kErrMsgNoMandatoryKey, key, error);
    return false;
  }
  bool result = ConvertValue(tmp, value);
  if (!result)
    SetError(kErrMsgInvalidKeyValue, key, error);
  return result;
}

// Retrieves an optional value from specified dictionary and specified key.
// If the value is found, the function returns true and fills value
// parameter. If the value is not found, the function returns true and fills
// value parameter with default value. If an error occurs, it returns false
// and fills error parameter if it is set.
template <typename ValueType>
bool GetOptionalValue(const parser::DictionaryValue& dict,
    const std::string& key, ValueType default_value, ValueType* value,
    std::string* error) {
  assert(value);
  std::string tmp;
  if (!dict.GetString(key, &tmp)) {
    *value = default_value;
    return true;
  }
  bool result = ConvertValue(tmp, value);
  if (!result)
    SetError(kErrMsgInvalidKeyValue, key, error);
  return result;
}

// Helper function for ParseEach. Do not use directly.
template <typename ParseSingleType, typename DataContainerType>
bool ParseEachInternal(const parser::Value& value,
    const std::string& key, ParseSingleType parse_single,
    DataContainerType* data_container, std::string* error) {
  assert(data_container);
  const parser::DictionaryValue* inner_dict;
  if (!value.GetAsDictionary(&inner_dict)) {
    SetError(kErrMsgInvalidDictionary, key, error);
    return false;
  }
  if (!parse_single(*inner_dict, key, data_container, error))
    return false;
  return true;
}

// Parsing helper function calling 'parse_single' for each dictionary contained
// in 'dict' under a 'key'. This helper function takes two template arguments:
//  - a function with following prototype:
//    bool ParseSingleExample(const parser::Value& value,
//    const std::string& key, DataContainerType* data_container,
//    std::string* error);
//  - a DataContainerType object where the above function stores data
template <typename ParseSingleType, typename DataContainerType>
bool ParseEach(const parser::DictionaryValue& dict,
               const std::string& key, bool mandatory,
               ParseSingleType parse_single, DataContainerType* data_container,
               std::string* error) {
  assert(data_container);

  const parser::Value* value = nullptr;
  if (!dict.Get(key, &value) || !value) {
    if (mandatory) {
      SetError(kErrMsgNoMandatoryKey, key, error);
      return false;
    }
    return true;
  }

  if (value->IsType(parser::Value::TYPE_DICTIONARY)) {
    if (!ParseEachInternal(*value, key, parse_single, data_container, error))
      return false;
  } else if (value->IsType(parser::Value::TYPE_LIST)) {
    const parser::ListValue* list;
    if (!value->GetAsList(&list)) {
      SetError(kErrMsgInvalidList, key, error);
      return false;
    }
    for (const parser::Value* value : *list)
      if (!ParseEachInternal(*value, key, parse_single, data_container, error))
        return false;
  }

  return true;
}

// Verifies whether specified dictionary represents an element in specified
// namespace. Returns true, if the namespace is set and equal to the specified
// one or false otherwise. If the error parameter is specified, it is also
// filled with proper message.
bool VerifyElementNamespace(const parser::DictionaryValue& dict,
    const std::string& key, const std::string& desired_namespace_value,
    std::string* error) {
  std::string namespace_value;
  if (!GetMandatoryValue(dict, keys::kNamespaceKey,
      &namespace_value, nullptr)) {
    SetError(kErrMsgNoNamespace, key, error);
    return false;
  }
  if (namespace_value != desired_namespace_value) {
    SetError(kErrMsgInvalidNamespace, key, error);
    return false;
  }
  return true;
}

// Parses box-label part
bool ParseLabel(const parser::DictionaryValue& dict,
    const std::string& key, AppWidget* app_widget, std::string* error) {
  assert(app_widget);

  if (!VerifyElementNamespace(dict, key, keys::kTizenNamespacePrefix, error))
    return false;

  std::string lang;
  if (!GetOptionalValue(dict, keys::kTizenAppWidgetBoxLabelLangKey,
      std::string(), &lang, error))
    return false;

  std::string text;
  if (!GetMandatoryValue(dict, keys::kTizenAppWidgetBoxLabelTextKey,
      &text, error))
    return false;

  if (lang.empty()) {
    // Note: Tizen 2.2 WRT Core Spec does not determine how many times the value
    // without lang attribute can appear in one app-widget, so overwrite.
    app_widget->label.default_value = text;
  } else {
    // Note: Tizen 2.2 WRT Core Spec does not determine how many times the value
    // with specific lang attribute can appear in one app-widget, so overwrite.
    app_widget->label.lang_value_map[lang] = text;
  }

  return true;
}

// Parses box-icon part
bool ParseIcon(const parser::DictionaryValue& dict,
    const std::string& key, AppWidget* app_widget, std::string* error) {
  assert(app_widget);

  if (!VerifyElementNamespace(dict, key, keys::kTizenNamespacePrefix, error))
    return false;

  if (!app_widget->icon_src.empty()) {
    SetError(kErrMsgMultipleKeys, key, error);
    return false;
  }
  if (!GetMandatoryValue(dict, keys::kTizenAppWidgetBoxIconSrcKey,
      &app_widget->icon_src, error))
    return false;

  return true;
}

// Converts size type from text to enum representation
bool StringToSizeType(const std::string& str_type,
    AppWidgetSizeType* enum_type) {
  assert(enum_type);
  if (str_type == "1x1") {
    *enum_type = AppWidgetSizeType::k1x1;
    return true;
  }
  if (str_type == "2x1") {
    *enum_type = AppWidgetSizeType::k2x1;
    return true;
  }
  if (str_type == "2x2") {
    *enum_type = AppWidgetSizeType::k2x2;
    return true;
  }
  return false;
}

// Parses box-size part
bool ParseContentSizes(const parser::DictionaryValue& dict,
    const std::string& key, AppWidget* app_widget, std::string* error) {
  assert(app_widget);

  if (!VerifyElementNamespace(dict, key, keys::kTizenNamespacePrefix, error))
    return false;

  AppWidgetSize size;

  std::string str_type;
  if (!GetMandatoryValue(dict, keys::kTizenAppWidgetBoxContentSizeTextKey,
      &str_type, error))
    return false;

  AppWidgetSizeType type;
  if (!StringToSizeType(str_type, &type)) {
    SetError(kErrMsgInvalidKeyValue,
        keys::kTizenAppWidgetBoxContentSizeTextKey, error);
    return false;
  }
  size.type = type;

  if (!GetOptionalValue(dict, keys::kTizenAppWidgetBoxContentSizePreviewKey,
      std::string(), &size.preview, error))
    return false;

  if (!GetOptionalValue(dict,
      keys::kTizenAppWidgetBoxContentSizeUseDecorationKey,
      true, &size.use_decoration, error))
    return false;

  app_widget->content_size.push_back(size);

  return true;
}

// Parses pd part
bool ParseContentDropView(const parser::DictionaryValue& dict,
    const std::string& key, AppWidget* app_widget, std::string* error) {
  assert(app_widget);

  if (!VerifyElementNamespace(dict, key, keys::kTizenNamespacePrefix, error))
    return false;

  if (!app_widget->content_drop_view.empty()) {
    SetError(kErrMsgMultipleKeys, key, error);
    return false;
  }

  AppWidgetDropView drop_view;

  if (!GetMandatoryValue(dict, keys::kTizenAppWidgetBoxContentDropViewSrcKey,
      &drop_view.src, error))
    return false;

  if (!GetMandatoryValue(dict,
      keys::kTizenAppWidgetBoxContentDropViewWidthKey,
      &drop_view.width, error))
    return false;

  if (!GetMandatoryValue(dict,
      keys::kTizenAppWidgetBoxContentDropViewHeightKey,
      &drop_view.height, error))
    return false;

  app_widget->content_drop_view.push_back(drop_view);

  return true;
}

// Parses box-content part
bool ParseContent(const parser::DictionaryValue& dict,
    const std::string& key, AppWidget* app_widget, std::string* error) {
  assert(app_widget);

  if (!VerifyElementNamespace(dict, key, keys::kTizenNamespacePrefix, error))
    return false;

  if (!app_widget->content_src.empty()) {
    SetError(kErrMsgMultipleKeys, key, error);
    return false;
  }
  if (!GetMandatoryValue(dict, keys::kTizenAppWidgetBoxContentSrcKey,
      &app_widget->content_src, error))
    return false;

  if (!GetOptionalValue(dict, keys::kTizenAppWidgetBoxContentMouseEventKey,
      false, &app_widget->content_mouse_event, error))
    return false;

  if (!GetOptionalValue(dict, keys::kTizenAppWidgetBoxContentTouchEffectKey,
      true, &app_widget->content_touch_effect, error))
    return false;

  if (!ParseEach(dict, keys::kTizenAppWidgetBoxContentSizeKey,
      true, ParseContentSizes, app_widget, error))
    return false;

  if (!ParseEach(dict, keys::kTizenAppWidgetBoxContentDropViewKey,
      false, ParseContentDropView, app_widget, error))
    return false;

  return true;
}

// Parses app-widget part
bool ParseAppWidget(const parser::DictionaryValue& dict,
    const std::string& key, AppWidgetVector* app_widgets,
    std::string* error) {
  assert(app_widgets);

  if (!VerifyElementNamespace(dict, key, keys::kTizenNamespacePrefix, error))
    return false;

  AppWidget app_widget;

  if (!GetMandatoryValue(dict, keys::kTizenAppWidgetIdKey,
      &app_widget.id, error))
    return false;

  if (!GetMandatoryValue(dict, keys::kTizenAppWidgetPrimaryKey,
      &app_widget.primary, error))
    return false;

  double update_period;
  double no_update_period = std::numeric_limits<double>::min();
  if (!GetOptionalValue(dict, keys::kTizenAppWidgetUpdatePeriodKey,
      no_update_period, &update_period, error))
    return false;
  if (update_period != no_update_period)
    app_widget.update_period.push_back(update_period);

  if (!GetOptionalValue(dict, keys::kTizenAppWidgetAutoLaunchKey,
      false, &app_widget.auto_launch, error))
    return false;

  if (!ParseEach(dict, keys::kTizenAppWidgetBoxLabelKey,
      true, ParseLabel, &app_widget, error))
    return false;

  if (!ParseEach(dict, keys::kTizenAppWidgetBoxIconKey,
      false, ParseIcon, &app_widget, error))
    return false;

  if (!ParseEach(dict, keys::kTizenAppWidgetBoxContentKey,
      true, ParseContent, &app_widget, error))
    return false;

  app_widgets->push_back(app_widget);

  return true;
}

// Tests if specified string represents valid remote url
bool IsValidUrl(const std::string& /*value*/) {
  // TODO(tweglarski): implement me (it's not crucial atm)
  return true;
}

// Tests if specified string represents valid path
bool IsValidPath(const std::string& /*value*/) {
  // TODO(tweglarski): implement me (it's not crucial atm)
  return true;
}

// Tests if specified string represents valid path or remote url
bool IsValidPathOrUrl(const std::string& value) {
  return IsValidPath(value) || IsValidUrl(value);
}

// Validates all content sizes in an app-widget
bool ValidateContentSize(const AppWidgetSizeVector& content_size,
    std::string* error) {
  bool mandatory_1x1_found = false;

  for (const AppWidgetSize& size : content_size) {
    mandatory_1x1_found |= size.type == AppWidgetSizeType::k1x1;

    if (!size.preview.empty() && !IsValidPath(size.preview)) {
      SetError(kErrMsgInvalidContentSizePreview, size.preview, error);
      return false;
    }
  }

  if (!mandatory_1x1_found) {
    SetError(kErrMsgNoMandatoryContentSize1x1, error);
    return false;
  }

  return true;
}

}  // namespace

AppWidgetInfo::AppWidgetInfo(const AppWidgetVector& app_widgets)
    : app_widgets_(app_widgets) {
}

AppWidgetInfo::~AppWidgetInfo() {
}

AppWidgetHandler::AppWidgetHandler() {
}

AppWidgetHandler::~AppWidgetHandler() {
}

bool AppWidgetHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  const parser::DictionaryValue* dict = nullptr;
  if (!GetMandatoryDictionary(manifest, keys::kTizenWidgetKey,
                              &dict, error))
    return false;

  AppWidgetVector app_widgets;

  if (!ParseEach(*dict, keys::kTizenAppWidgetKey,
      false, ParseAppWidget, &app_widgets, error))
    return false;

  *output = std::static_pointer_cast<parser::ManifestData>(
      std::make_shared<AppWidgetInfo>(app_widgets));
  return true;
}

bool AppWidgetHandler::Validate(
    const parser::ManifestData& data,
    const parser::ManifestDataMap& /*handlers_output*/,
    std::string* error) const {
  const AppWidgetInfo& app_widget_info =
      static_cast<const AppWidgetInfo&>(data);
  const AppWidgetVector& app_widgets = app_widget_info.app_widgets();

  for (const AppWidget& app_widget : app_widgets) {
    if (!app_widget.update_period.empty()
        && app_widget.update_period.front() < 1800) {
      SetError(kErrMsgUpdatePeriodOutOfDomain,
          std::to_string(app_widget.update_period.front()), error);
      return false;
    }

    if (app_widget.label.default_value.empty()
        && app_widget.label.lang_value_map.empty()) {
      SetError(kErrMsgNoLabel, error);
      return false;
    }

    if (!app_widget.icon_src.empty()
        && !IsValidPathOrUrl(app_widget.icon_src)) {
      SetError(kErrMsgInvalidIconSrc, app_widget.icon_src, error);
      return false;
    }

    if (!IsValidPathOrUrl(app_widget.content_src)) {
      SetError(kErrMsgInvalidContentSrc, app_widget.content_src, error);
      return false;
    }

    if (!ValidateContentSize(app_widget.content_size, error))
      return false;

    if (!app_widget.content_drop_view.empty()) {
      const AppWidgetDropView& drop_view
          = app_widget.content_drop_view.front();

      if (!IsValidPathOrUrl(drop_view.src)) {
        SetError(kErrMsgInvalidContentDropViewSrc, drop_view.src, error);
        return false;
      }

      if (drop_view.height < 1 || drop_view.height > 380) {
        SetError(kErrMsgContentDropViewHeightOutOfDomain,
            std::to_string(drop_view.height), error);
        return false;
      }
    }
  }

  return true;
}

std::string AppWidgetHandler::Key() const {
  return keys::kTizenAppWidgetFullKey;
}

}  // namespace parse
}  // namespace wgt
