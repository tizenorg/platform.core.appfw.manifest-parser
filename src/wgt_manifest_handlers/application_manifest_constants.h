// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef WGT_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
#define WGT_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_

#include "manifest_parser/manifest.h"

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


// Keys used in JSON representation of applications.
namespace wgt {

namespace application_widget_keys {

LIBSCL_EXPORT_API extern const char kAccessKey[];
LIBSCL_EXPORT_API extern const char kAccountKey[];
LIBSCL_EXPORT_API extern const char kAllowNavigationKey[];
LIBSCL_EXPORT_API extern const char kAuthorKey[];
LIBSCL_EXPORT_API extern const char kCSPKey[];
LIBSCL_EXPORT_API extern const char kCSPReportOnlyKey[];
LIBSCL_EXPORT_API extern const char kDescriptionKey[];
LIBSCL_EXPORT_API extern const char kIconsKey[];
LIBSCL_EXPORT_API extern const char kTizenApplicationAppControlsKey[];
LIBSCL_EXPORT_API extern const char kTizenApplicationKey[];
LIBSCL_EXPORT_API extern const char kTizenAppWidgetFullKey[];
LIBSCL_EXPORT_API extern const char kTizenBackgroundCategoryKey[];
LIBSCL_EXPORT_API extern const char kTizenCategoryKey[];
LIBSCL_EXPORT_API extern const char kTizenContentKey[];
LIBSCL_EXPORT_API extern const char kTizenImeKey[];
LIBSCL_EXPORT_API extern const char kTizenMetaDataKey[];
LIBSCL_EXPORT_API extern const char kTizenPermissionsKey[];
LIBSCL_EXPORT_API extern const char kTizenServiceKey[];
LIBSCL_EXPORT_API extern const char kTizenSettingKey[];
LIBSCL_EXPORT_API extern const char kTizenSplashScreenKey[];
LIBSCL_EXPORT_API extern const char kTizenWidgetKey[];
LIBSCL_EXPORT_API extern const char kVersionKey[];
LIBSCL_EXPORT_API extern const char kWidgetKey[];
LIBSCL_EXPORT_API extern const char kSplashScreenKey[];

}  // namespace application_widget_keys

}  // namespace wgt

#endif  // WGT_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
