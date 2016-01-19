// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_COMMON_APPLICATION_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_COMMON_APPLICATION_HANDLER_H_

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/utils/iri_util.h"
#include "manifest_parser/utils/version_number.h"
#include "manifest_parser/values.h"
#include "tpk_manifest_handlers/ui_and_service_application_infos.h"

namespace tpk {
namespace parse {
namespace tpk_app_keys {
// app-control
extern const char kAppControlKey[];
extern const char kAppControlOperationKey[];
extern const char kAppControlURIKey[];
extern const char kAppControlMimeKey[];
extern const char kAppControlNameChildKey[];

// background-category
extern const char kBackgroundCategoryKey[];
extern const char kBackgroundCategoryValueKey[];

// datacontrol
extern const char kDataControlKey[];
extern const char kDataControlAccessKey[];
extern const char kDataControlProviderIDKey[];
extern const char kDataControlTypeKey[];

// icon
extern const char kIconKey[];
extern const char kIconTextKey[];

// label
extern const char kLabelKey[];
extern const char kLabelTextKey[];
extern const char kLabelLangKey[];

// metadata
extern const char kMetaDataKey[];
extern const char kMetaDataKeyKey[];
extern const char kMetaDataValueKey[];
}  // namespace tpk_app_keys

extern const utils::VersionNumber kLaunchModeRequiredVersion;

class ApplicationInfo : public parser::ManifestData {
 public:
  void set_appid(const std::string& appid) {
    appid_ = appid;
  }

  void set_exec(const std::string& exec) {
    exec_ = exec;
  }

  const std::string& appid() const {
    return appid_;
  }

  const std::string& exec() const {
    return exec_;
  }

 private:
  std::string appid_;
  std::string exec_;
};

template<typename T>
struct ApplicationSingleEntry : public parser::ManifestData {
  T app_info;
  std::vector<MetaDataInfo> meta_data;
  ApplicationIconsInfo app_icons;
  std::vector<LabelInfo> label;
};

template<typename T>
struct ApplicationInfoList : public parser::ManifestData {
  std::vector<T> items;
};

template<typename T>
bool ParseAppControl(const parser::DictionaryValue& dict,
                     T* info, std::string*) {
  for (const auto& item_operation : parser::GetOneOrMany(&dict,
      tpk_app_keys::kAppControlOperationKey, "")) {
    std::string operation;
    std::string uri;
    std::string mime;
    item_operation->GetString(
        tpk_app_keys::kAppControlNameChildKey, &operation);

    auto uri_items = parser::GetOneOrMany(
        &dict, tpk_app_keys::kAppControlURIKey, "");
    auto mime_items = parser::GetOneOrMany(
        &dict, tpk_app_keys::kAppControlMimeKey, "");

    if (uri_items.empty()) uri_items.push_back(new parser::DictionaryValue());
    if (mime_items.empty()) mime_items.push_back(new parser::DictionaryValue());

    for (const auto& item_uri : uri_items) {
      item_uri->GetString(tpk_app_keys::kAppControlNameChildKey, &uri);

      for (const auto& item_mime : mime_items) {
        item_mime->GetString(tpk_app_keys::kAppControlNameChildKey, &mime);
        info->app_control.emplace_back(operation, uri, mime);
      }
    }
  }

  return true;
}

template<typename T>
bool ParseBackgroundCategoryElement(const parser::DictionaryValue& dict,
                                    T* info, std::string* error) {
  std::string value;

  if (!dict.GetString(tpk_app_keys::kBackgroundCategoryValueKey, &value)) {
    *error = "Parsing background-category element failed";
    return false;
  }

  info->background_category.emplace_back(std::move(value));

  return true;
}

template<typename T>
bool ParseDataControl(const parser::DictionaryValue& dict,
                      T* info, std::string*) {
  std::string access;
  dict.GetString(tpk_app_keys::kDataControlAccessKey, &access);
  std::string providerid;
  dict.GetString(tpk_app_keys::kDataControlProviderIDKey, &providerid);
  std::string type;
  dict.GetString(tpk_app_keys::kDataControlTypeKey, &type);
  info->data_control.emplace_back(access, providerid, type);
  return true;
}

template<typename T>
bool ParseAppIcon(const parser::DictionaryValue& dict,
                  T* info, std::string* /*error*/) {
  std::string icon_path;

  if (!dict.GetString(tpk_app_keys::kIconTextKey, &icon_path))
    return true;

  info->app_icons.AddIcon(ApplicationIcon(icon_path));

  return true;
}

template<typename T>
bool ParseLabel(const parser::DictionaryValue& dict,
                T* info, std::string*) {
  std::string text;
  dict.GetString(tpk_app_keys::kLabelTextKey, &text);
  std::string xml_lang;
  dict.GetString(tpk_app_keys::kLabelLangKey, &xml_lang);
  info->label.emplace_back(text, text, xml_lang);
  return true;
}

template<typename T>
bool ParseMetaData(const parser::DictionaryValue& dict,
                   T* info, std::string*) {
  std::string key;
  dict.GetString(tpk_app_keys::kMetaDataKeyKey, &key);
  std::string val;
  dict.GetString(tpk_app_keys::kMetaDataValueKey, &val);

  if (key.find(tpk_app_keys::kBackgroundCategoryKey) == std::string::npos) {
    info->meta_data.emplace_back(key, val);
  } else {
    val = key.substr(key.find_last_of("/") + 1);
    info->background_category.emplace_back(std::move(val));
  }

  return true;
}

template<typename T>
using ParsingFuncPtr =
    std::function<bool(const parser::DictionaryValue&, T*, std::string*)>;

template<typename T>
bool InitializeParsingElement(
    const parser::DictionaryValue& app_dict,
    const std::string& key,
    ParsingFuncPtr<T> ParsingFunc,
    T* applicationinfo, std::string* error) {
  for (const auto& item : parser::GetOneOrMany(&app_dict, key, "")) {
    if (!ParsingFunc(*item, applicationinfo, error)) {
      return false;
    }
  }
  return true;
}

template<typename T>
bool AppControlValidation(const T& it, std::string* error) {
  for (const auto& item : it.app_control) {
    if (item.operation().empty()) {
      *error =
          "The operation child element of app-control element is obligatory";
      return false;
    } else if (!parser::utils::IsValidIRI(item.operation())) {
      *error =
          "The operation child element of app-control element is not valid url";
      return false;
    }
  }
  return true;
}

template<typename T>
bool DataControlValidation(const T& it, std::string* error) {
  for (const auto& item : it.data_control) {
    if (item.access().empty()) {
      *error =
          "The access child element of datacontrol element is obligatory";
      return false;
    }

    if (item.providerid().empty()) {
      *error =
          "The providerid child element of datacontrol element is obligatory";
      return false;
    }

    if (item.type().empty()) {
      *error =
          "The type child element of datacontrol element is obligatory";
      return false;
    }
  }
  return true;
}

template<typename T>
bool LabelValidation(const T& it, std::string* error) {
  if (std::any_of(it.label.begin(), it.label.end(),
      [](const LabelInfo& item) { return item.text().empty(); })) {
    *error = "The text child element of label element is obligatory";
    return false;
  }

  if (std::any_of(it.label.begin(), it.label.end(),
      [](const LabelInfo& item) { return item.name().empty(); })) {
    *error = "The name child element of label element is obligatory";
    return false;
  }

  return true;
}

template<typename T>
bool MetadataValidation(const T& it, std::string* error) {
  if (std::any_of(it.meta_data.begin(), it.meta_data.end(),
      [](const MetaDataInfo& item) { return item.key().empty(); })) {
    *error = "The key child element of metadata element is obligatory";
    return false;
  }

  return true;
}

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_COMMON_APPLICATION_HANDLER_H_
