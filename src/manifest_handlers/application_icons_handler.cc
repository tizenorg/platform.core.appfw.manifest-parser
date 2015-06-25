// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "manifest_handlers/application_icons_handler.h"

#include <algorithm>
#include <string>

#include "utils/logging.h"

namespace wgt {
namespace parse {

namespace keys = wgt::application_widget_keys;

const std::vector<std::string>& ApplicationIconsInfo::get_icon_paths() const {
  return icon_paths_;
}

void ApplicationIconsInfo::add_icon_path(const std::string& icon_path) {
  // Eliminate duplicates, keep order
  if (std::find(icon_paths_.begin(), icon_paths_.end(), icon_path)
      != icon_paths_.end()) {
    return;
  }
  icon_paths_.push_back(icon_path);
}

ApplicationIconsHandler::ApplicationIconsHandler() {
}

ApplicationIconsHandler::~ApplicationIconsHandler() {
}

bool ApplicationIconsHandler::ExtractIconSrc(
    const parser::Value& dict, std::string* value,
    std::string* error) {
  const parser::DictionaryValue* inner_dict;
  if (!dict.GetAsDictionary(&inner_dict)) {
    LOG(INFO) << "Cannot get key value as a dictionary. Key name: widget.icon";
    return true;
  }
  std::string src;
  if (!inner_dict->GetString(keys::kWidgetIconSrcKey, &src)) {
    LOG(INFO) << "Cannot find mandatory key. Key name: .@src";
    return true;
  }
  *value = src;
  return  true;
}

bool ApplicationIconsHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  std::shared_ptr<ApplicationIconsInfo> app_icons_info =
      std::make_shared<ApplicationIconsInfo>();
  parser::Value* key_value;
  if (!manifest.Get(keys::kWidgetIconKey, &key_value)) {
    *output = std::static_pointer_cast<parser::ManifestData>(app_icons_info);
    return true;
  }

  if (key_value->IsType(parser::Value::TYPE_DICTIONARY)) {
    std::string icon;
    if (!ExtractIconSrc(*key_value, &icon, error)) {
      *error = "Cannot get key value as a dictionary. Key name: widget.icon";
      return false;
    }
    app_icons_info->add_icon_path(icon);
  } else if (key_value->IsType(parser::Value::TYPE_LIST)) {
    const parser::ListValue* list;
    if (!key_value->GetAsList(&list)) {
      *error = "Cannot get key value as a list. Key name: widget.icon";
      return false;
    }
    for (const parser::Value* list_value : *list) {
      std::string icon;
      if (!ExtractIconSrc(*list_value, &icon, error)) {
        *output =
            std::static_pointer_cast<parser::ManifestData>(app_icons_info);
        return true;
      }
      app_icons_info->add_icon_path(icon);
    }
  }
  *output = std::static_pointer_cast<parser::ManifestData>(app_icons_info);
  return true;
}

bool ApplicationIconsHandler::AlwaysParseForType() const {
  return true;
}

std::string ApplicationIconsHandler::Key() const {
  return wgt::application_manifest_keys::kIconsKey;
}

}  // namespace parse
}  // namespace wgt
