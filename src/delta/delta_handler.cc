// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "delta/delta_handler.h"

namespace {

const char kAddFilesKey[] = "add-files";
const char kModifyFilesKey[] = "modify-files";
const char kRemoveFilesKey[] = "remove-files";
const char kFileNameKey[] = "@name";
const char kFileKey[] = "file";

bool GetFilesForKey(const parser::DictionaryValue* dict, const std::string& key,
                    std::vector<std::string>* files, std::string* error) {
  for (const auto& item : parser::GetOneOrMany(dict, key, "")) {
    for (const auto& file : parser::GetOneOrMany(item, kFileKey, "")) {
      std::string position;
      if (!file->GetString(kFileNameKey, &position)) {
        *error = std::string("Malformed tag: ") + key;
        return false;
      }
      files->push_back(position);
    }
  }
  return true;
}

}  // namespace

namespace delta {

const char kDeltaInfoKey[] = "delta";

bool DeltaHandler::Parse(
    const parser::Manifest& manifest,
    std::shared_ptr<parser::ManifestData>* output,
    std::string* error) {
  parser::Value* value = nullptr;
  if (!manifest.Get(kDeltaInfoKey, &value)) {
    *error = "Cannot find root element <delta>";
    return false;
  }
  const parser::DictionaryValue* dict = nullptr;
  if (!value->GetAsDictionary(&dict)) {
    *error = "Root element malformed";
    return false;
  }
  std::shared_ptr<DeltaInfo> delta_info(new DeltaInfo());

  std::vector<std::string> added;
  if (!GetFilesForKey(dict, kAddFilesKey, &added, error))
    return false;
  delta_info->set_added(added);

  std::vector<std::string> modified;
  if (!GetFilesForKey(dict, kModifyFilesKey, &modified, error))
    return false;
  delta_info->set_modified(modified);

  std::vector<std::string> removed;
  if (!GetFilesForKey(dict, kRemoveFilesKey, &removed, error))
    return false;
  delta_info->set_removed(removed);

  *output = std::static_pointer_cast<parser::ManifestData>(delta_info);
  return true;
}

std::string DeltaHandler::Key() const {
  return kDeltaInfoKey;
}

}  // namespace delta
