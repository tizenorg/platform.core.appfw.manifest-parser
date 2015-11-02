// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
#define MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_

#include "manifest_parser/manifest.h"

// Keys used in JSON representation of applications.
namespace wgt {
namespace application_manifest_keys {

// TODO(p.sikorski): below key should be removed, when nwrt and app-installer
// will stop use it. Now, it is just copied.
extern const char kIconsKey[];

}  // namespace application_manifest_keys

namespace application_widget_keys {

//extern const char kAccessKey[];
//extern const char kAllowNavigationKey[];
//extern const char kCSPKey[];
//extern const char kCSPReportOnlyKey[];
//extern const char kTizenApplicationAppControlsKey[];
//extern const char kTizenApplicationKey[];
//extern const char kTizenCategoryKey[];
//extern const char kTizenContentKey[];
//extern const char kTizenMetaDataKey[];
//extern const char kTizenPermissionsKey[];
//extern const char kTizenSettingKey[];
//extern const char kTizenSplashScreenKey[];
//extern const char kTizenWidgetKey[];


//extern const char kAccountKey[];
//extern const char kAuthorKey[];
//extern const char kDescriptionKey[];
//extern const char kIconsKey[];
//extern const char kWidgetKey[];
//extern const char kTizenAppWidgetFullKey[];
//extern const char kTizenImeKey[];
//extern const char kTizenServiceKey[];
//extern const char kVersionKey[];


}  // namespace application_widget_keys

}  // namespace wgt

#endif  // MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
