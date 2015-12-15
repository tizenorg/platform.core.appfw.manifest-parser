// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_COMMON_APPLICATION_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_COMMON_APPLICATION_HANDLER_H_

#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/utils/iri_util.h"
#include "tpk_manifest_handlers/ui_and_service_application_infos.h"

namespace {
// app-control
const char kAppControlKey[] = "app-control";
const char kAppControlOperationKey[] = "operation";
const char kAppControlURIKey[] = "uri";
const char kAppControlMimeKey[] = "mime";
const char kAppControlNameChildKey[] = "@name";

// background-category
const char kBackgroundCategoryKey[] = "background-category";
const char kBackgroundCategoryValueKey[] = "@value";

// datacontrol
const char kDataControlKey[] = "datacontrol";
const char kDataControlAccessKey[] = "@access";
const char kDataControlProviderIDKey[] = "@providerid";
const char kDataControlTypeKey[] = "@type";

// icon
const char kIconKey[] = "icon";
const char kIconTextKey[] = "#text";

// label
const char kLabelKey[] = "label";
const char kLabelTextKey[] = "#text";
const char kLabelLangKey[] = "@lang";

// metadata
const char kMetaDataKey[] = "metadata";
const char kMetaDataKeyKey[] = "@key";
const char kMetaDataValueKey[] = "@value";
}  // namespace

namespace tpk {
namespace parse {

class ApplicationInfo : public parser::ManifestData {
 public:
  /**
   * @brief set_appid sets appid
   * @param appid
   */
  void set_appid(const std::string& appid);
  /**
   * @brief set_exec sets exec
   * @param exec
   */
  void set_exec(const std::string& exec);
  /**
   * @brief set_type sets type
   * @param type
   */
  void set_type(const std::string& type);
  /**
   * @brief set_process_pool sets process_pool
   * @param process_pool
   */
  void set_process_pool(const std::string& process_pool);

  /**
   * @brief appid
   * @return appid string
   */
  const std::string& appid() const;
  /**
   * @brief exec
   * @return exec string
   */
  const std::string& exec() const;
  /**
   * @brief type
   * @return type string
   */
  const std::string& type() const;
  /**
   * @brief process_pool
   * @return process_pool string
   */
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

template<typename T>
bool ParseAppControl(const parser::DictionaryValue* dict, T* info) {
  std::string operation;
  const parser::DictionaryValue* operation_dict;
  if (dict->GetDictionary(kAppControlOperationKey,
                                 &operation_dict)) {
    operation_dict->GetString(
        kAppControlNameChildKey, &operation);
  }

  std::string uri;
  const parser::DictionaryValue* uri_dict;
  if (dict->GetDictionary(kAppControlURIKey,
                                 &uri_dict)) {
    uri_dict->GetString(
        kAppControlNameChildKey, &uri);
  }

  std::string mime;
  const parser::DictionaryValue* mime_dict;
  if (dict->GetDictionary(kAppControlMimeKey,
                                 &mime_dict)) {
    mime_dict->GetString(
        kAppControlNameChildKey, &mime);
  }

  info->app_control.emplace_back(operation, uri, mime);
  return true;
}

template<typename T>
bool ParseBackgroundCategoryElement(const parser::DictionaryValue* dict,
    T* info) {
  std::string value;

  if (!dict->GetString(kBackgroundCategoryValueKey, &value))
    return false;

  info->background_category.emplace_back(std::move(value));

  return true;
}

template<typename T>
bool ParseDataControl(const parser::DictionaryValue* dict, T* info) {
  std::string access;
  dict->GetString(kDataControlAccessKey, &access);
  std::string providerid;
  dict->GetString(kDataControlProviderIDKey, &providerid);
  std::string type;
  dict->GetString(kDataControlTypeKey, &type);
  info->data_control.emplace_back(access, providerid, type);
  return true;
}

template<typename T>
bool ParseAppIcon(const parser::DictionaryValue* dict, T* info) {
  std::string icon_path;
  if (!dict->GetString(kIconTextKey, &icon_path))
    return false;
  info->app_icons.AddIcon(ApplicationIcon(icon_path));
  return true;
}

template<typename T>
bool ParseLabel(const parser::DictionaryValue* dict, T* info) {
  std::string text;
  dict->GetString(kLabelTextKey, &text);
  std::string xml_lang;
  dict->GetString(kLabelLangKey, &xml_lang);
  info->label.emplace_back(text, text, xml_lang);
  return true;
}

template<typename T>
bool ParseMetaData(const parser::DictionaryValue* dict, T* info) {
  std::string key;
  dict->GetString(kMetaDataKeyKey, &key);
  std::string val;
  dict->GetString(kMetaDataValueKey, &val);

  if (key.find(kBackgroundCategoryKey) == std::string::npos) {
    info->meta_data.emplace_back(key, val);
  } else {
    val = key.substr(key.find_last_of("/") + 1);
    info->background_category.emplace_back(std::move(val));
  }

  return true;
}

template<typename T>
bool InitializeAppControlParsing(
    const parser::DictionaryValue& app_dict,
    T* applicationinfo, std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kAppControlKey, "")) {
    if (!ParseAppControl(item, applicationinfo)) {
      *error = "Parsing AppControl failed";
      return false;
    }
  }
  return true;
}

template<typename T>
bool InitializeBackgroundCategoryParsing(
    const parser::DictionaryValue& control_dict,
    T* applicationinfo, std::string* error) {
  for (auto& item : parser::GetOneOrMany(&control_dict,
      kBackgroundCategoryKey, "")) {
    if (!ParseBackgroundCategoryElement(item, applicationinfo)) {
      *error = "Parsing background-category element failed";
      return false;
    }
  }
  return true;
}

template<typename T>
bool InitializeDataControlParsing(
    const parser::DictionaryValue& app_dict,
    T* applicationinfo, std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kDataControlKey, "")) {
    if (!ParseDataControl(item, applicationinfo)) {
      *error = "Parsing DataControl failed";
      return false;
    }
  }
  return true;
}

template<typename T>
bool InitializeIconParsing(
    const parser::DictionaryValue& app_dict,
    T* applicationinfo, std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kIconKey, "")) {
    if (!ParseAppIcon(item, applicationinfo)) {
      *error = "Parsing Icon failed";
      return false;
    }
  }
  return true;
}

template<typename T>
bool InitializeLabelParsing(
    const parser::DictionaryValue& app_dict,
    T* applicationinfo, std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kLabelKey, "")) {
    if (!ParseLabel(item, applicationinfo)) {
      *error = "Parsing Label failed";
      return false;
    }
  }
  return true;
}

template<typename T>
bool InitializeMetaDataParsing(
    const parser::DictionaryValue& app_dict,
    T* applicationinfo, std::string* error) {
  for (auto& item : parser::GetOneOrMany(&app_dict, kMetaDataKey, "")) {
    if (!ParseMetaData(item, applicationinfo)) {
      *error = "Parsing Metadata failed";
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

}  // parse
}  // tpk

#endif  // TPK_MANIFEST_HANDLERS_COMMON_APPLICATION_HANDLER_H_
