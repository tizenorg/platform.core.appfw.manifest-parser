// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache-2.0 license that can be
// found in the LICENSE file.

#include <boost/filesystem/path.hpp>

#include <gtest/gtest.h>

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
}

TEST_F(ServiceHandlerTest, SingleServiceEntryOnBootOn) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
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
}

TEST_F(ServiceHandlerTest, SingleServiceEntryAutoRestartOn) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
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
}

TEST_F(ServiceHandlerTest, SingleServiceEntryWrongId) {
  // Set test values
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  std::unique_ptr<DictionaryValue> widget(new DictionaryValue());
  std::unique_ptr<DictionaryValue> service(new DictionaryValue());
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
  service->SetInteger(keys::kTizenServiceIdKey, 1410);
  service->SetString(keys::kNamespaceKey, keys::kTizenNamespacePrefix);
  widget->Set(kServiceKey, service.release());
  value->Set(keys::kWidgetKey, widget.release());
  std::shared_ptr<Manifest> manifest(new Manifest(std::move(value)));
  SetManifest(manifest);
  // Check correctness
  ASSERT_FALSE(ParseAppManifest());
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
}

}  // namespace parser

