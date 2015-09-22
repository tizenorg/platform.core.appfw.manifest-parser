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
extern const char kIconsKey[];
extern const char kAccountKey[];
extern const char kAccountMASKey[];
extern const char kAccountIconKey[];
extern const char kAccountNameKey[];
extern const char kAccountSectionKey[];
extern const char kAccountTextKey[];
extern const char kAccountCapabilityKey[];
extern const char kAccountLangKey[];
extern const char kNamespaceKey[];
extern const char kXmlLangKey[];
extern const char kXmlHrefKey[];
extern const char kDefaultLocaleKey[];
extern const char kNameKey[];
extern const char kWebURLsKey[];
extern const char kWidgetKey[];
extern const char kVersionKey[];
extern const char kViewModesKey[];
extern const char kAccessKey[];
extern const char kLicenseKey[];
extern const char kAccessOriginKey[];
extern const char kAccessSubdomainsKey[];
extern const char kCSPKey[];
extern const char kCSPKeyReportOnly[];
extern const char kAuthorKey[];
extern const char kDescriptionKey[];
extern const char kShortNameKey[];
extern const char kShortKey[];
extern const char kIDKey[];
extern const char kAuthorEmailKey[];
extern const char kAuthorHrefKey[];
extern const char kHeightKey[];
extern const char kWidthKey[];
extern const char kPreferencesKey[];
extern const char kPreferencesNameKey[];
extern const char kPreferencesValueKey[];
extern const char kPreferencesReadonlyKey[];
extern const char kWidgetLangKey[];
extern const char kWidgetNamespaceKey[];
extern const char kWidgetNamespacePrefix[];
extern const char kTizenWidgetKey[];
extern const char kTizenApplicationKey[];
extern const char kTizenApplicationIdKey[];
extern const char kTizenApplicationLaunchModeKey[];
extern const char kTizenApplicationPackageKey[];
extern const char kTizenApplicationRequiredVersionKey[];
extern const char kTizenAppIdKey[];
extern const char kWidgetIconKey[];
extern const char kWidgetIconSrcKey[];
extern const char kWidgetIconWidthKey[];
extern const char kWidgetIconHeightKey[];
extern const char kAllowNavigationKey[];
extern const char kCSPReportOnlyKey[];
extern const char kTizenSettingKey[];
extern const char kTizenBackgroundSupportKey[];
extern const char kTizenContextMenuKey[];
extern const char kTizenHardwareKey[];
extern const char kTizenEncryptionKey[];
extern const char kTizenInstallLocationKey[];
extern const char kTizenNoDisplayKey[];
extern const char kTizenIndicatorPresenceKey[];
extern const char kTizenBackbuttonPresenceKey[];
extern const char kTizenUserAgentKey[];
extern const char kTizenSoundModeKey[];
extern const char kTizenBackgroundVibrationKey[];
extern const char kTizenMetaDataKey[];
extern const char kTizenMetaDataNameKey[];
extern const char kTizenMetaDataValueKey[];
extern const char kTizenPermissionsKey[];
extern const char kTizenPermissionsNameKey[];
extern const char kTizenSplashScreenKey[];
extern const char kTizenSplashScreenSrcKey[];
extern const char kContentNamespace[];
extern const char kTizenScreenOrientationKey[];
extern const char kTizenAppWidgetFullKey[];
extern const char kTizenAppWidgetKey[];
extern const char kTizenAppWidgetIdKey[];
extern const char kTizenAppWidgetPrimaryKey[];
extern const char kTizenAppWidgetUpdatePeriodKey[];
extern const char kTizenAppWidgetAutoLaunchKey[];
extern const char kTizenAppWidgetBoxLabelKey[];
extern const char kTizenAppWidgetBoxLabelLangKey[];
extern const char kTizenAppWidgetBoxLabelTextKey[];
extern const char kTizenAppWidgetBoxIconKey[];
extern const char kTizenAppWidgetBoxIconSrcKey[];
extern const char kTizenAppWidgetBoxContentKey[];
extern const char kTizenAppWidgetBoxContentSrcKey[];
extern const char kTizenAppWidgetBoxContentMouseEventKey[];
extern const char kTizenAppWidgetBoxContentTouchEffectKey[];
extern const char kTizenAppWidgetBoxContentSizeKey[];
extern const char kTizenAppWidgetBoxContentSizeTextKey[];
extern const char kTizenAppWidgetBoxContentSizePreviewKey[];
extern const char kTizenAppWidgetBoxContentSizeUseDecorationKey[];
extern const char kTizenAppWidgetBoxContentDropViewKey[];
extern const char kTizenAppWidgetBoxContentDropViewSrcKey[];
extern const char kTizenAppWidgetBoxContentDropViewWidthKey[];
extern const char kTizenAppWidgetBoxContentDropViewHeightKey[];
extern const char kTizenApplicationAppControlsKey[];
extern const char kTizenApplicationAppControlSrcKey[];
extern const char kTizenApplicationAppControlOperationKey[];
extern const char kTizenApplicationAppControlUriKey[];
extern const char kTizenApplicationAppControlMimeKey[];
extern const char kTizenApplicationAppControlOnResetKey[];
extern const char kTizenApplicationAppControlChildNameAttrKey[];
extern const char kTizenImeKey[];
extern const char kTizenImeUuidKey[];
extern const char kTizenImeUuidTextKey[];
extern const char kTizenImeLanguagesKey[];
extern const char kTizenImeLanguageKey[];
extern const char kTizenImeLanguageTextKey[];
extern const char kTizenContentKey[];
extern const char kTizenContentSrcKey[];
extern const char kTizenContentEncodingKey[];
extern const char kTizenContentTypeKey[];
extern const char kTizenCategoryKey[];
extern const char kTizenCategoryNameKey[];
extern const char kTizenNamespacePrefix[];
extern const char kTizenServiceKey[];
extern const char kTizenServiceIdKey[];
extern const char kTizenServiceAutoRestartKey[];
extern const char kTizenServiceOnBootKey[];
extern const char kXmlTextKey[];
}  // namespace application_widget_keys

}  // namespace wgt

#endif  // MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
