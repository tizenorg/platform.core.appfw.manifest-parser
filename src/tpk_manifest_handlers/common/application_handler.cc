// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/common/application_handler.h"

namespace tpk {
namespace parse {
namespace tpk_app_keys {
// app-control
const char kAppControlKey[] = "app-control";
const char kAppControlOperationKey[] = "operation";
const char kAppControlURIKey[] = "uri";
const char kAppControlMimeKey[] = "mime";
const char kAppControlNameChildKey[] = "@name";

// background-category
const char kBackgroundCategoryKey[] = "background-category";
const char kBackgroundCategoryValueKey[] = "@value";

// datacontrol
const char kDataControlKey[] = "datacontrol";
const char kDataControlAccessKey[] = "@access";
const char kDataControlProviderIDKey[] = "@providerid";
const char kDataControlTypeKey[] = "@type";

// icon
const char kIconKey[] = "icon";
const char kIconTextKey[] = "#text";
const char kIconLangKey[] = "@lang";
const char kIconDpiKey[] = "@dpi";

// image
const char kImageKey[] = "image";
const char kImageNameKey[] = "@name";
const char kImageSectionKey[] = "@section";
const char kImageLangKey[] = "@lang";

// label
const char kLabelKey[] = "label";
const char kLabelTextKey[] = "#text";
const char kLabelLangKey[] = "@lang";

// metadata
const char kMetaDataKey[] = "metadata";
const char kMetaDataKeyKey[] = "@key";
const char kMetaDataValueKey[] = "@value";

// category
const char kCategoryKey[] = "category";
const char kCategoryNameKey[] = "@name";

}  // namespace tpk_app_keys

const utils::VersionNumber kLaunchModeRequiredVersion("2.4");

}  // namespace parse
}  // namespace tpk
