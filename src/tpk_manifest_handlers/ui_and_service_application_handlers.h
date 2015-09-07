// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_UI_AND_SERVICE_APPLICATION_HANDLERS_H_
#define TPK_MANIFEST_HANDLERS_UI_AND_SERVICE_APPLICATION_HANDLERS_H_

#include <memory>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"

namespace tpk {
namespace parse {

// AppControl
class AppControlInfo : public parser::ManifestData {
 public:
  AppControlInfo(const std::string& operation,
                 const std::string& uri,
                 const std::string& mime)
                 : operation_(operation),
                   uri_(uri),
                   mime_(mime) {}

  const std::string& operation() const {
    return operation_;
  }

  const std::string& uri() const {
    return uri_;
  }

  const std::string& mime() const {
    return mime_;
  }

 private:
  std::string operation_;
  std::string uri_;
  std::string mime_;
};

// DataControl
class DataControlInfo : public parser::ManifestData {
 public:
  DataControlInfo(const std::string& access,
                  const std::string& providerid,
                  const std::string& type)
                  : access_(access),
                    providerid_(providerid),
                    type_(type) {}

  const std::string& access() const {
    return access_;
  }
  const std::string& providerid() const {
    return providerid_;
  }
  const std::string& type() const {
    return type_;
  }

 private:
  std::string access_;
  std::string providerid_;
  std::string type_;
};

// MetaData
class MetaDataInfo : public parser::ManifestData {
 public:
  MetaDataInfo(const std::string& key,
               const std::string& val)
               : key_(key),
                 value_(val) {}

  const std::string& key() const {
    return key_;
  }
  const std::string& val() const {
    return value_;
  }

 private:
  std::string key_;
  std::string value_;
};

// Icon
class ApplicationIcon  {
 public:
  explicit ApplicationIcon(const std::string& path)
                          : path_(path) {}

  const std::string& path() const {
    return path_;
  }

 private:
  std::string path_;
};

class ApplicationIconsInfo : public parser::ManifestData {
 public:
  const std::vector<ApplicationIcon>& icons() const {
  return icons_;
  }

  void AddIcon(const ApplicationIcon& icon_path);

 private:
  std::vector<ApplicationIcon> icons_;
};

// Label
class LabelInfo : public parser::ManifestData {
 public:
  LabelInfo(const std::string& text,
            const std::string& name,
            const std::string& xml_lang)
            : text_(text),
              name_(name),
              xml_lang_(xml_lang) {}

  const std::string& text() const {
    return text_;
  }
  const std::string& name() const {
    return name_;
  }
  const std::string& xml_lang() const {
    return xml_lang_;
  }

 private:
  std::string text_;
  std::string name_;
  std::string xml_lang_;
};
}   // namespace parse
}   // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_UI_AND_SERVICE_APPLICATION_HANDLERS_H_
