// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/application_icons_handler.h"

#include <algorithm>
#include <string>

#include "utils/logging.h"

namespace keys = tpk::application_keys;

namespace {

bool ExtractIconSrc(const parser::Value& dict, std::string* value,
                    std::string* error) {
  const parser::DictionaryValue* inner_dict;
  if (!dict.GetAsDictionary(&inner_dict)) {
    *error = "Cannot get key value as a dictionary. Key name: icon";
    return true;
  }
  std::string src;
  if (!inner_dict->GetString(keys::kIconSrcKey, &src)) {
    *error = "Cannot find mandatory key. Key name: @src";
    return true;
  }
  *value = src;
  return  true;
}
}  // namespace

namespace tpk {
namespace parse {

ApplicationIcon::ApplicationIcon(const std::string& path)
    : path_(path) {
}

const std::string& ApplicationIcon::path() const {
  return path_;
}

const std::vector<ApplicationIcon>& ApplicationIconsInfo::icons() const {
  return icons_;
}

void ApplicationIconsInfo::AddIcon(const ApplicationIcon& new_icon) {
  // Eliminate duplicates, keep order
  if (std::find_if(icons_.begin(), icons_.end(),
      [&new_icon](const ApplicationIcon& icon) {
        return icon.path() == new_icon.path();
      })
      != icons_.end()) {
    return;
  }
  icons_.push_back(new_icon);
}

bool ApplicationIconsHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<ApplicationIconsInfo> app_icons_info =
      std::make_shared<ApplicationIconsInfo>();
  parser::Value* key_value;
  if (!manifest.Get(keys::kIconKey, &key_value)) {
    *output = std::static_pointer_cast<parser::ManifestData>(app_icons_info);
    return true;
  }

  if (key_value->IsType(parser::Value::TYPE_DICTIONARY)) {
    std::string icon_path;
    if (!ExtractIconSrc(*key_value, &icon_path, error)) {
      *error = "Cannot get key value as a dictionary. Key name: icon";
      return false;
    }
    app_icons_info->AddIcon(ApplicationIcon(icon_path));
  } else if (key_value->IsType(parser::Value::TYPE_LIST)) {
    const parser::ListValue* list;
    if (!key_value->GetAsList(&list)) {
      *error = "Cannot get key value as a list. Key name: icon";
      return false;
    }
    for (const parser::Value* list_value : *list) {
      std::string icon_path;
      if (!ExtractIconSrc(*list_value, &icon_path, error)) {
        *output =
            std::static_pointer_cast<parser::ManifestData>(app_icons_info);
        return true;
      }
      app_icons_info->AddIcon(ApplicationIcon(icon_path));
    }
  }
  *output = std::static_pointer_cast<parser::ManifestData>(app_icons_info);
  return true;
}

bool ApplicationIconsHandler::AlwaysParseForType() const {
  return true;
}

std::string ApplicationIconsHandler::Key() const {
  return tpk::application_keys::kIconsKey;
}

}  // namespace parse
}  // namespace tpk
