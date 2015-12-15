// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_COMMON_APPLICATION_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_COMMON_APPLICATION_HANDLER_H_

#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/utils/iri_util.h"
#include "tpk_manifest_handlers/ui_and_service_application_infos.h"

namespace tpk {
namespace parse {
namespace app_keys {
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
}  // namespace app_keys

class ApplicationInfo : public parser::ManifestData {
 public:
  void set_appid(const std::string& appid);
  void set_exec(const std::string& exec);
  void set_type(const std::string& type);
  void set_process_pool(const std::string& process_pool);

  const std::string& appid() const;
  const std::string& exec() const;
  const std::string& type() const;
  const std::string& process_pool() const;

 private:
  std::string appid_;
  std::string exec_;
  std::string type_;
  std::string process_pool_;
};

template<typename T>
struct ApplicationSingleEntry : public parser::ManifestData {
  T app_info;
  std::vector<AppControlInfo> app_control;
  std::vector<BackgroundCategoryInfo> background_category;
  std::vector<DataControlInfo> data_control;
  std::vector<MetaDataInfo> meta_data;
  ApplicationIconsInfo app_icons;
  std::vector<LabelInfo> label;
};

template<typename T>
struct ApplicationInfoList : public parser::ManifestData {
  std::vector<T> items;
};

std::string GetAppControlValue(const parser::DictionaryValue& dict,
                               std::string key);

template<typename T>
bool ParseAppControl(const parser::DictionaryValue& dict,
                     T* info, std::string*) {
  std::string operation = GetAppControlValue(dict,
      app_keys::kAppControlOperationKey);
  std::string uri = GetAppControlValue(dict, app_keys::kAppControlURIKey);
  std::string mime = GetAppControlValue(dict, app_keys::kAppControlMimeKey);

  info->app_control.emplace_back(operation, uri, mime);
  return true;
}

template<typename T>
bool ParseBackgroundCategoryElement(const parser::DictionaryValue& dict,
                                    T* info, std::string* error) {
  std::string value;

  if (!dict.GetString(app_keys::kBackgroundCategoryValueKey, &value)) {
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
  dict.GetString(app_keys::kDataControlAccessKey, &access);
  std::string providerid;
  dict.GetString(app_keys::kDataControlProviderIDKey, &providerid);
  std::string type;
  dict.GetString(app_keys::kDataControlTypeKey, &type);
  info->data_control.emplace_back(access, providerid, type);
  return true;
}

template<typename T>
bool ParseAppIcon(const parser::DictionaryValue& dict,
                  T* info, std::string* error) {
  std::string icon_path;

  if (!dict.GetString(app_keys::kIconTextKey, &icon_path)) {
    *error = "Parsing Icon failed";
    return false;
  }

  info->app_icons.AddIcon(ApplicationIcon(icon_path));

  return true;
}

template<typename T>
bool ParseLabel(const parser::DictionaryValue& dict,
                T* info, std::string*) {
  std::string text;
  dict.GetString(app_keys::kLabelTextKey, &text);
  std::string xml_lang;
  dict.GetString(app_keys::kLabelLangKey, &xml_lang);
  info->label.emplace_back(text, text, xml_lang);
  return true;
}

template<typename T>
bool ParseMetaData(const parser::DictionaryValue& dict,
                   T* info, std::string*) {
  std::string key;
  dict.GetString(app_keys::kMetaDataKeyKey, &key);
  std::string val;
  dict.GetString(app_keys::kMetaDataValueKey, &val);

  if (key.find(app_keys::kBackgroundCategoryKey) == std::string::npos) {
    info->meta_data.emplace_back(key, val);
  } else {
    val = key.substr(key.find_last_of("/") + 1);
    info->background_category.emplace_back(std::move(val));
  }

  return true;
}

template<typename T>
using ParsingFuncPtr = bool (*)(
    const parser::DictionaryValue&, T*, std::string*);

template<typename T>
bool InitializeParsingElement(
    const parser::DictionaryValue& app_dict,
    const std::string key,
    ParsingFuncPtr<T> ParsingFunc,
    T* applicationinfo, std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, key, "")) {
    if (!(*ParsingFunc)(*item, applicationinfo, error)) {
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
    }
    if (!parser::utils::IsValidIRI(item.operation())) {
      *error =
          "The operation child element of app-control element is not valid url";
      return false;
    }
    const std::string& uri = item.uri();
    if (!uri.empty()) {
      if (!parser::utils::IsValidIRI(uri)) {
        *error =
            "The uri child element of app-control element is not valid url";
        return false;
      }
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

    const std::string& providerid = item.providerid();
    if (providerid.empty()) {
      *error =
          "The providerid child element of datacontrol element is obligatory";
      return false;
    }

    const std::string& type = item.type();
    if (type.empty()) {
      *error =
          "The type child element of datacontrol element is obligatory";
      return false;
    }
  }
  return true;
}

template<typename T>
bool LabelValidation(const T& it, std::string* error) {
  for (const auto& item : it.label) {
    if (item.text().empty()) {
      *error = "The text child element of label element is obligatory";
      return false;
    }

    const std::string& name = item.name();
    if (name.empty()) {
      *error =
          "The name child element of label element is obligatory";
      return false;
    }
  }
  return true;
}

template<typename T>
bool MetadataValidation(const T& it, std::string* error) {
  for (const auto& item : it.meta_data) {
    if (item.key().empty()) {
      *error =
          "The key child element of metadata element is obligatory";
      return false;
    }
  }
  return true;
}

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_COMMON_APPLICATION_HANDLER_H_
