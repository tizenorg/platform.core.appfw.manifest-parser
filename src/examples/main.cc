// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

// Include manifest parser
#include <manifest_parser/manifest_parser.h>
// Include manifest handler, which is packaged with manifest parser

#include <boost/filesystem/path.hpp>
#include <cassert>
#include <memory>

#include "custom_handlers/handler_constants.h"
// Include our custom handlers defined in application
#include "custom_handlers/item_list_handler.h"
#include "custom_handlers/item_handler.h"

int main() {
  boost::filesystem::path manifest_path =
      "/usr/share/manifest-parser/config.xml";

  // Create manifest handler registry
  std::unique_ptr<parser::ManifestHandlerRegistry> registry(
      new parser::ManifestHandlerRegistry());
  // Add some manifest handlers
  registry->RegisterManifestHandler(new example::ItemListHandler);
  registry->RegisterManifestHandler(new example::ItemHandler);

  // There is another way of adding ManifestHandlers:
  // std::vector<parser::ManifestHandler*> handlers = {
  //   new wgt::parse::TizenApplicationHandler
  //   new example::ItemListHandler
  //   new example::ItemHandler
  // }
  //
  // std::unique_ptr<parser::ManifestHandlerRegistry> registry;
  // registry.reset(new parser::ManifestHandlerRegistry(handlers));

  std::unique_ptr<parser::ManifestParser> m_parser(new parser::ManifestParser(
      std::move(registry)));
  if (!m_parser->ParseManifest(manifest_path)) {
    fprintf(stderr, "Parsing failed, err message: %s\n",
            m_parser->GetErrorMessage().c_str());
    return -1;
  }

  // Test parsed manifest
  std::shared_ptr<const example::ItemInfo> item_info =
      std::static_pointer_cast<const example::ItemInfo>(
          m_parser->GetManifestData(example::keys::item_handler_key));
  std::shared_ptr<const example::ItemListInfo> item_info_list =
      std::static_pointer_cast<const example::ItemListInfo>(
          m_parser->GetManifestData(example::keys::item_handler_list_key));

  // Check parsed ItemInfo data
  assert(!strcmp("additional", item_info->type().c_str()));
  assert(!strcmp("standalone", item_info->value().c_str()));

  // Check parsed ItemInfoList data
  assert(!strcmp("text", item_info_list->get_item(0)->type().c_str()));
  assert(!strcmp("first", item_info_list->get_item(0)->value().c_str()));
  assert(!strcmp("file", item_info_list->get_item(1)->type().c_str()));
  assert(!strcmp("second.png", item_info_list->get_item(1)->value().c_str()));
  assert(!strcmp("text", item_info_list->get_item(2)->type().c_str()));
  assert(!strcmp("third", item_info_list->get_item(2)->value().c_str()));

  return 0;
}
