// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache-2.0 license that can be
// found in the LICENSE file.

#include <boost/filesystem/path.hpp>

#include <gtest/gtest.h>

#include <initializer_list>
#include <vector>

#include "manifest_handlers/application_manifest_constants.h"
#include "manifest_handlers/service_handler.h"
#include "manifest_parser/manifest.h"
#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/manifest_parser_impl.h"
#include "manifest_parser/manifest_parser.h"

namespace bf = boost::filesystem;

namespace {

const char kNamespaceKey[] = "@namespace";
const char kServiceKey[] = "service";

std::unique_ptr<parser::ManifestHandlerRegistry> GetRegistryForTest() {
  std::unique_ptr<parser::ManifestHandlerRegistry> registry;
  registry.reset(new parser::ManifestHandlerRegistry());
  registry->RegisterManifestHandler(new wgt::parse::ServiceHandler());
  return registry;
}

}  // namespace

namespace parser {

namespace keys = wgt::application_widget_keys;

class ServiceHandlerTest : public testing::Test {
 public:
  void SetUp() override {
    parser_.reset(new ManifestParserImpl((GetRegistryForTest())));
  }
  void TearDown() override {
    parser_.reset();
  }
  void SetManifest(std::shared_ptr<Manifest> manifest) {
    parser_->manifest_ = manifest;
  }
  bool ParseAppManifest() {
    std::string error;
    return parser_->ParseAppManifest(&error);
  }
  bool ValidateAppManifest() {
    std::string error;
    return parser_->ValidateAppManifest(&error);
  }
  std::shared_ptr<const ManifestData> GetManifestData(const std::string& key) {
    return parser_->GetManifestData(key);
  }

 private:
  std::unique_ptr<ManifestParserImpl> parser_;
};

TEST_F(ServiceHandlerTest, NoServiceEntry) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntryDefault) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].categories().size(), 0);
  ASSERT_EQ(service_list->services[0].metadata_set().size(), 0);
}

TEST_F(ServiceHandlerTest, SingleServiceEntryOnBootOn) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct002.appId");
  service->SetString(keys::kTizenServiceOnBootKey, "true");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct002.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), true);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
}

TEST_F(ServiceHandlerTest, SingleServiceEntryAutoRestartOn) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct003.appId");
  service->SetString(keys::kTizenServiceOnBootKey, "false");
  service->SetString(keys::kTizenServiceAutoRestartKey, "true");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct003.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), true);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
}

TEST_F(ServiceHandlerTest, SingleServiceEntryWrongId) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "wrongid.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_FALSE(ValidateAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntryIdTypeMismatch) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetInteger(keys::kTizenServiceIdKey, 1410);
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_FALSE(ParseAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntryNameMissing) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->SetInteger(keys::kTizenServiceIdKey, 1410);
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_FALSE(ParseAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntryIdSingleNameNotInTizen) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->SetInteger(keys::kTizenServiceIdKey, 1410);
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_FALSE(ParseAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntryMultipleNames) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);

  std::unique_ptr<ListValue> name_list(new ListValue());
  for (auto& pair : {std::make_pair(std::string(), "first"),
                     std::make_pair(std::string("en"), "second"),
                     std::make_pair(std::string("de"), "third")}) {
    std::unique_ptr<DictionaryValue> name(new DictionaryValue());
    name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
    if (!pair.first.empty())
      name->SetString(keys::kXmlLangKey, pair.first);
    name->SetString(keys::kXmlTextKey, pair.second);
    name_list->Append(name.release());
  }
  service->Set(keys::kTizenServiceNameKey, name_list.release());

  service->Set(keys::kTizenServiceContentKey, content.release());
  service->SetString(keys::kTizenServiceIdKey, "correct003.appId");
  service->SetString(keys::kTizenServiceOnBootKey, "false");
  service->SetString(keys::kTizenServiceAutoRestartKey, "true");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct003.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), true);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 3);
  ASSERT_EQ(service_list->services[0].names()[0].first, "");
  ASSERT_EQ(service_list->services[0].names()[0].second, "first");
  ASSERT_EQ(service_list->services[0].names()[1].first, "en");
  ASSERT_EQ(service_list->services[0].names()[1].second, "second");
  ASSERT_EQ(service_list->services[0].names()[2].first, "de");
  ASSERT_EQ(service_list->services[0].names()[2].second, "third");
}

TEST_F(ServiceHandlerTest, SingleServiceEntryIdSingleContentNotInTizen) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetInteger(keys::kTizenServiceIdKey, 1410);
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_FALSE(ParseAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntryContentMissing) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetInteger(keys::kTizenServiceIdKey, 1410);
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_FALSE(ParseAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntryMultipleContents) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());

  std::unique_ptr<ListValue> content_list(new ListValue());
  for (auto& start_file : {"content1.js", "content2.js"}) {
    std::unique_ptr<DictionaryValue> content(new DictionaryValue());
    content->SetString(keys::kTizenServiceContentSrcKey, start_file);
    content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
    content_list->Append(content.release());
  }
  service->Set(keys::kTizenServiceContentKey, content_list.release());

  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetInteger(keys::kTizenServiceIdKey, 1410);
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_FALSE(ParseAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntrySingleIcon) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  std::unique_ptr<DictionaryValue> icon(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  icon->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  icon->SetString(keys::kTizenServiceIconSrcKey, "my_icon.png");
  service->Set(keys::kTizenServiceIconKey, icon.release());
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].icon(), "my_icon.png");
}

TEST_F(ServiceHandlerTest, SingleServiceEntrySingleIconNotInTizen) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  std::unique_ptr<DictionaryValue> icon(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  icon->SetString(keys::kTizenServiceIconSrcKey, "my_icon.png");
  service->Set(keys::kTizenServiceIconKey, icon.release());
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].icon(), "");
}

TEST_F(ServiceHandlerTest, SingleServiceEntryMultipleIcon) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");

  std::unique_ptr<ListValue> icon_list(new ListValue());
  for (auto& icon_value : {"icon1.png", "icon2.png"}) {
    std::unique_ptr<DictionaryValue> icon(new DictionaryValue());
    icon->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
    icon->SetString(keys::kTizenServiceIconSrcKey, icon_value);
    icon_list->Append(icon.release());
  }

  service->Set(keys::kTizenServiceIconKey, icon_list.release());
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_FALSE(ParseAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntrySingleDescription) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  std::unique_ptr<DictionaryValue> description(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  description->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  description->SetString(keys::kXmlTextKey, "my description");
  service->Set(keys::kTizenServiceDescriptionKey, description.release());
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].description(), "my description");
}

TEST_F(ServiceHandlerTest, SingleServiceEntrySingleDescriptionNotInTizen) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  std::unique_ptr<DictionaryValue> description(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  description->SetString(keys::kXmlTextKey, "my description");
  service->Set(keys::kTizenServiceDescriptionKey, description.release());
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].description(), "");
}

TEST_F(ServiceHandlerTest, SingleServiceEntryMultipleDescription) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");

  std::unique_ptr<ListValue> description_list(new ListValue());
  for (auto& desc_value : {"1", "2"}) {
    std::unique_ptr<DictionaryValue> description(new DictionaryValue());
    description->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
    description->SetString(keys::kXmlTextKey, desc_value);
    description_list->Append(description.release());
  }
  service->Set(keys::kTizenServiceDescriptionKey, description_list.release());

  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_FALSE(ParseAppManifest());
}

TEST_F(ServiceHandlerTest, SingleServiceEntrySingleMetadata) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  std::unique_ptr<DictionaryValue> metadata(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  metadata->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  metadata->SetString(keys::kTizenServiceMetadataKeyKey, "unique key");
  metadata->SetString(keys::kTizenServiceMetadataValueKey, "some value");
  service->Set(keys::kTizenServiceMetadataKey, metadata.release());
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].metadata_set().size(), 1);
  ASSERT_EQ(service_list->services[0].metadata_set()[0].first, "unique key");
  ASSERT_EQ(service_list->services[0].metadata_set()[0].second, "some value");
}

TEST_F(ServiceHandlerTest, SingleServiceEntrySingleMetadataNotInTizen) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  std::unique_ptr<DictionaryValue> metadata(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  metadata->SetString(keys::kTizenServiceMetadataKeyKey, "unique key");
  metadata->SetString(keys::kTizenServiceMetadataValueKey, "some value");
  service->Set(keys::kTizenServiceMetadataKey, metadata.release());
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].metadata_set().size(), 0);
}

TEST_F(ServiceHandlerTest, SingleServiceEntryMultipleMetadata) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");

  std::unique_ptr<ListValue> metadata_list(new ListValue());
  std::unique_ptr<DictionaryValue> metadata(new DictionaryValue());
  metadata->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  metadata->SetString(keys::kTizenServiceMetadataKeyKey, "unique key");
  metadata->SetString(keys::kTizenServiceMetadataValueKey, "some value");
  metadata_list->Append(metadata.release());
  std::unique_ptr<DictionaryValue> metadata2(new DictionaryValue());
  metadata2->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  metadata2->SetString(keys::kTizenServiceMetadataKeyKey, "unique key 2");
  metadata_list->Append(metadata2.release());
  service->Set(keys::kTizenServiceMetadataKey, metadata_list.release());

  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].metadata_set().size(), 2);
  ASSERT_EQ(service_list->services[0].metadata_set()[0].first, "unique key");
  ASSERT_EQ(service_list->services[0].metadata_set()[0].second, "some value");
  ASSERT_EQ(service_list->services[0].metadata_set()[1].first, "unique key 2");
  ASSERT_EQ(service_list->services[0].metadata_set()[1].second, "");
}

TEST_F(ServiceHandlerTest, SingleServiceEntryMultipleMetadataMixedNamespaces) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");

  std::unique_ptr<ListValue> metadata_list(new ListValue());

  std::unique_ptr<DictionaryValue> metadata(new DictionaryValue());
  metadata->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  metadata->SetString(keys::kTizenServiceMetadataKeyKey, "unique key");
  metadata->SetString(keys::kTizenServiceMetadataValueKey, "some value");
  metadata_list->Append(metadata.release());

  std::unique_ptr<DictionaryValue> metadata2(new DictionaryValue());
  metadata2->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  metadata2->SetString(keys::kTizenServiceMetadataKeyKey, "unique key 2");
  metadata_list->Append(metadata2.release());

  std::unique_ptr<DictionaryValue> metadata3(new DictionaryValue());
  metadata3->SetString(keys::kTizenServiceMetadataKeyKey, "unique key 3");
  metadata_list->Append(metadata3.release());

  service->Set(keys::kTizenServiceMetadataKey, metadata_list.release());

  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].metadata_set().size(), 2);
  ASSERT_EQ(service_list->services[0].metadata_set()[0].first, "unique key");
  ASSERT_EQ(service_list->services[0].metadata_set()[0].second, "some value");
  ASSERT_EQ(service_list->services[0].metadata_set()[1].first, "unique key 2");
  ASSERT_EQ(service_list->services[0].metadata_set()[1].second, "");
}

TEST_F(ServiceHandlerTest, SingleServiceEntrySingleCategory) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  std::unique_ptr<DictionaryValue> category(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  category->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  category->SetString(keys::kTizenServiceCategoryNameKey, "category name");
  service->Set(keys::kTizenServiceCategoryKey, category.release());
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].categories().size(), 1);
  ASSERT_EQ(service_list->services[0].categories()[0], "category name");
}

TEST_F(ServiceHandlerTest, SingleServiceEntrySingleCategoryNotInTizen) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  std::unique_ptr<DictionaryValue> category(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");
  category->SetString(keys::kTizenServiceCategoryNameKey, "category name");
  service->Set(keys::kTizenServiceCategoryKey, category.release());
  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].categories().size(), 0);
}

TEST_F(ServiceHandlerTest, SingleServiceEntryMultipleCategory) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
  std::unique_ptr<DictionaryValue> content(new DictionaryValue());
  std::unique_ptr<DictionaryValue> name(new DictionaryValue());
  content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
  content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  name->SetString(keys::kXmlTextKey, "name");

  std::unique_ptr<ListValue> catogory_list(new ListValue());
  for (auto& name : {"category name 1", "category name 2", "category name 3"}) {
    std::unique_ptr<DictionaryValue> category(new DictionaryValue());
    category->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
    category->SetString(keys::kTizenServiceCategoryNameKey, name);
    catogory_list->Append(category.release());
  }
  service->Set(keys::kTizenServiceCategoryKey, catogory_list.release());

  service->Set(keys::kTizenServiceContentKey, content.release());
  service->Set(keys::kTizenServiceNameKey, name.release());
  service->SetString(keys::kTizenServiceIdKey, "correct001.appId");
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 1);
  ASSERT_EQ(service_list->services[0].id(), "correct001.appId");
  ASSERT_EQ(service_list->services[0].auto_restart(), false);
  ASSERT_EQ(service_list->services[0].on_boot(), false);
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[0].categories().size(), 3);
  ASSERT_EQ(service_list->services[0].categories()[0], "category name 1");
  ASSERT_EQ(service_list->services[0].categories()[1], "category name 2");
  ASSERT_EQ(service_list->services[0].categories()[2], "category name 3");
}

TEST_F(ServiceHandlerTest, MultipleServiceEntry) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<ListValue> list(new ListValue());
  std::unique_ptr<DictionaryValue> service1(
      new DictionaryValue());
  std::unique_ptr<DictionaryValue> service2(
      new DictionaryValue());
  service1->SetString(keys::kTizenServiceIdKey, "correct004.appId");
  service1->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  service2->SetString(keys::kTizenServiceIdKey, "correct005.appId");
  service2->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  for (auto& service : {service1.get(), service2.get()}) {
    std::unique_ptr<DictionaryValue> content(new DictionaryValue());
    std::unique_ptr<DictionaryValue> name(new DictionaryValue());
    content->SetString(keys::kTizenServiceContentSrcKey, "service.js");
    content->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
    name->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
    name->SetString(keys::kXmlTextKey, "name");
    service->Set(keys::kTizenServiceContentKey, content.release());
    service->Set(keys::kTizenServiceNameKey, name.release());
  }
  list->Append(service1.release());
  list->Append(service2.release());
  widget->Set(kServiceKey, list.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_TRUE(ParseAppManifest());
  ASSERT_TRUE(ValidateAppManifest());
  ASSERT_TRUE(!!GetManifestData(keys::kTizenServiceKey));
  std::shared_ptr<const wgt::parse::ServiceList> service_list =
      std::dynamic_pointer_cast<const wgt::parse::ServiceList>(
          GetManifestData(keys::kTizenServiceKey));
  ASSERT_TRUE(!!service_list);
  ASSERT_EQ(service_list->services.size(), 2);
  ASSERT_EQ(service_list->services[0].id(), "correct004.appId");
  ASSERT_EQ(service_list->services[1].id(), "correct005.appId");
  ASSERT_EQ(service_list->services[0].content(), "service.js");
  ASSERT_EQ(service_list->services[0].names().size(), 1);
  ASSERT_EQ(service_list->services[0].names()[0].second, "name");
  ASSERT_EQ(service_list->services[1].content(), "service.js");
  ASSERT_EQ(service_list->services[1].names().size(), 1);
  ASSERT_EQ(service_list->services[1].names()[0].second, "name");
}

}  // namespace parser

