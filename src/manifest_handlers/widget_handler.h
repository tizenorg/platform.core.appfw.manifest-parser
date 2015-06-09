// Copyright (c) 2014 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_HANDLERS_WIDGET_HANDLER_H_
#define MANIFEST_HANDLERS_WIDGET_HANDLER_H_

#include <map>
#include <string>
#include <vector>

#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"

namespace wgt {
namespace parse {

class Preference {
 public:
  Preference(const std::string& name, const std::string& value,
             bool readonly)
      : name_(name),
        value_(value),
        readonly_(readonly) {}

  std::string Name() const { return name_; }
  std::string Value() const { return value_; }
  bool ReadOnly() const { return readonly_; }
 private:
  std::string name_;
  std::string value_;
  bool readonly_;
};

class WidgetInfo : public parser::ManifestData {
 public:
  WidgetInfo();
  virtual ~WidgetInfo();

  // Name, short name and description are i18n items, they will be set
  // if their value were changed after loacle was changed.
  void AddName(const std::string& locale, const std::string& name);
  void AddShortName(const std::string& locale, const std::string& short_name);
  void AddDescription(const std::string& locale,
                      const std::string& description);

  const std::map<std::string, std::string>& name_set() const;
  const std::map<std::string, std::string>& short_name_set() const;
  const std::map<std::string, std::string>& description_set() const;

  const std::string& id() const;
  const std::string& version() const;
  const std::string& view_modes() const;
  const std::string& default_locale() const;
  const std::string& author() const;
  const std::string& author_email() const;
  const std::string& author_href() const;
  const std::string& widget_namespace() const;
  const std::vector<Preference*>& preferences() const;
  unsigned int height() const;
  unsigned int width() const;

 private:
  std::vector<Preference*> preferences_;
  std::string id_;
  std::string version_;
  std::string viewmodes_;
  std::string default_locale_;
  std::string author_;
  std::map<std::string, std::string> description_set_;
  std::map<std::string, std::string> name_set_;
  std::map<std::string, std::string> short_name_set_;
  std::string author_email_;
  std::string author_href_;
  std::string widget_namespace_;
  unsigned int height_;
  unsigned int width_;

  friend class WidgetHandler;
};

class WidgetHandler : public parser::ManifestHandler {
 public:
  WidgetHandler();
  virtual ~WidgetHandler();

  bool Parse(
      const parser::Manifest& manifest,
      std::shared_ptr<parser::ManifestData>* output,
      std::string* error) override;
  bool AlwaysParseForType() const override;
  std::string Key() const override;

  void ParseSingleLocalizedDescriptionElement(
      const parser::DictionaryValue* item_dict,
      const std::string& parent_lang,
      std::shared_ptr<WidgetInfo> info);
  void ParseLocalizedDescriptionElements(
      const parser::Manifest& manifest,
      const std::string& parent_lang,
      std::shared_ptr<WidgetInfo> info);
  void ParseSingleLocalizedNameElement(
      const parser::DictionaryValue* item_dict,
      const std::string& parent_lang,
      std::shared_ptr<WidgetInfo> info);
  void ParseLocalizedNameElements(
      const parser::Manifest& manifest,
      const std::string& parent_lang,
      std::shared_ptr<WidgetInfo> info);

  bool Validate(
      const parser::ManifestData& data,
      const parser::ManifestDataMap& handlers_output,
      std::string* error) const override;

  private:
  bool ValidateIRIType(const std::string& prop) const;
};

}  // namespace parse
}  // namespace wgt

#endif  // MANIFEST_HANDLERS_WIDGET_HANDLER_H_
