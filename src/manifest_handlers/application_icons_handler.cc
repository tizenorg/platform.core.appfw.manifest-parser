// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "manifest_handlers/application_icons_handler.h"

#include <algorithm>
#include <string>

#include "utils/logging.h"

namespace keys = wgt::application_widget_keys;

namespace {

bool ExtractIconSrc(const parser::Value& dict, std::string* value,
                    std::string* /*error*/) {
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

/**
 * @brief ExtractIconDimensions
 *
 * Parses and sets icon dimensions. If value cannot be received it is set to -1.
 *
 * @param dict dictionary value of icon
 * @param height output parameter of height of icon
 * @param width output parameter of width of icon
 */
void ExtractIconDimensions(const parser::Value& dict, int* height, int* width) {
  *width = -1;
  *height = -1;
  const parser::DictionaryValue* inner_dict;
  if (!dict.GetAsDictionary(&inner_dict)) {
    return;
  }

  std::string width_str;
  if (inner_dict->GetString(keys::kWidgetIconWidthKey, &width_str)) {
    try {
      *width = std::stoi(width_str);
    } catch (const std::logic_error&) {
       // ignore, width is set to -1
    }
  }

  std::string height_str;
  if (inner_dict->GetString(keys::kWidgetIconHeightKey, &height_str)) {
    try {
      *height = std::stoi(height_str);
    } catch (const std::logic_error&) {
      // ignore, height is set to -1
    }
  }
}

}  // namespace

namespace wgt {
namespace parse {

ApplicationIcon::ApplicationIcon(const std::string& path, int height, int width)
    : path_(path),
      height_((height >= 0) ? height : -1),
      width_((width >= 0) ? width : -1) {
}

bool ApplicationIcon::GetWidth(int* width) const {
  if (!width || width_ < 0)
    return false;
  *width = width_;
  return true;
}

bool ApplicationIcon::GetHeight(int* height) const {
  if (!height || height_ < 0)
    return false;
  *height = height_;
  return true;
}

const std::string& ApplicationIcon::path() const {
  return path_;
}

void ApplicationIcon::set_width(int width) {
  if (width >= 0)
    width_ = width;
}

void ApplicationIcon::set_height(int height) {
  if (height >= 0)
    height_ = height;
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

ApplicationIconsHandler::ApplicationIconsHandler() {
}

ApplicationIconsHandler::~ApplicationIconsHandler() {
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
    std::string icon_path;
    int width = -1;
    int height = -1;
    if (!ExtractIconSrc(*key_value, &icon_path, error)) {
      *error = "Cannot get key value as a dictionary. Key name: widget.icon";
      return false;
    }
    ExtractIconDimensions(*key_value, &height, &width);
    app_icons_info->AddIcon(ApplicationIcon(icon_path, height, width));
  } else if (key_value->IsType(parser::Value::TYPE_LIST)) {
    const parser::ListValue* list;
    if (!key_value->GetAsList(&list)) {
      *error = "Cannot get key value as a list. Key name: widget.icon";
      return false;
    }
    for (const parser::Value* list_value : *list) {
      std::string icon_path;
      int width = -1;
      int height = -1;
      if (!ExtractIconSrc(*list_value, &icon_path, error)) {
        *output =
            std::static_pointer_cast<parser::ManifestData>(app_icons_info);
        return true;
      }
      ExtractIconDimensions(*key_value, &height, &width);
      app_icons_info->AddIcon(ApplicationIcon(icon_path, height, width));
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
