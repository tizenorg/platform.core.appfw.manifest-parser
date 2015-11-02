// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_SHORTCUT_HANDLER_H_
#define TPK_MANIFEST_HANDLERS_SHORTCUT_HANDLER_H_

#include <string>
#include <utility>
#include <vector>

#include "manifest_parser/manifest_handler.h"

namespace tpk {
namespace parse {

struct ShortcutInfo {
  std::string app_id;
  std::string extra_data;
  std::string extra_key;
  std::string icon;
  std::vector<std::pair<std::string, std::string>> labels;
};

class ShortcutListInfo : public parser::ManifestData {
 public:
  static std::string key();
  std::vector<ShortcutInfo> shortcuts;
};

class ShortcutHandler : public parser::ManifestHandler {
 public:
  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  std::string Key() const override;
};

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_SHORTCUT_HANDLER_H_
