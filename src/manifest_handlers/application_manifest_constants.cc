// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/application_manifest_constants.h"

namespace wgt {

namespace application_manifest_keys {

// TODO(p.sikorski): below key should be removed, when nwrt and app-installer
// will stop use it. Now, it is just copied.
const char kIconsKey[] = "icons";

}  // namespace application_manifest_keys

// manifest keys for widget applications.
namespace application_widget_keys {

const char kIconsKey[] = "icons";
const char kAccountKey[] = "widget.account";
const char kAccountMASKey[] = "@multiple-account-support";
const char kAccountIconKey[] = "icon";
const char kAccountNameKey[] = "display-name";
const char kAccountSectionKey[] = "@section";
const char kAccountTextKey[] = "#text";
const char kAccountCapabilityKey[] = "capability";
const char kAccountLangKey[] = "@lang";
const char kNamespaceKey[] = "@namespace";
const char kXmlLangKey[] = "@lang";
const char kXmlHrefKey[] = "@href";
const char kDefaultLocaleKey[] = "widget.@defaultlocale";
const char kNameKey[] = "widget.name";
const char kVersionKey[] = "widget.@version";
const char kViewModesKey[] = "widget.@viewmodes";
const char kWidgetKey[] = "widget";
const char kWebURLsKey[] = "widget.@id";
const char kAuthorKey[] = "widget.author";
const char kDescriptionKey[] = "widget.description";
const char kShortNameKey[] = "widget.name.@short";
const char kShortKey[] = "@short";
const char kIDKey[] = "widget.@id";
const char kAuthorEmailKey[] = "@email";
const char kAuthorHrefKey[] = "@href";
const char kHeightKey[] = "widget.@height";
const char kWidthKey[] = "widget.@width";
const char kPreferencesKey[] = "widget.preference";
const char kCSPKey[] = "widget.content-security-policy";
const char kCSPKeyReportOnly[] =
    "widget.content-security-policy-report-only";
const char kAccessKey[] = "widget.access";
const char kLicenseKey[] = "widget.license";

// Child keys inside 'kPreferencesKey'.
const char kPreferencesNameKey[] = "@name";
const char kPreferencesValueKey[] = "@value";
const char kPreferencesReadonlyKey[] = "@readonly";

const char kWidgetLangKey[] = "widget.@lang";
const char kWidgetNamespaceKey[] = "widget.@namespace";
const char kWidgetNamespacePrefix[] = "http://www.w3.org/ns/widgets";

// Child keys inside 'kAccessKey'.
const char kAccessOriginKey[] = "@origin";
const char kAccessSubdomainsKey[] = "@subdomains";

const char kTizenWidgetKey[] = "widget";

// W3C icon
const char kWidgetIconKey[] = "widget.icon";
const char kWidgetIconSrcKey[] = "@src";
const char kWidgetIconWidthKey[] = "@width";
const char kWidgetIconHeightKey[] = "@height";

const char kTizenApplicationKey[] = "widget.application";
// Child keys inside 'kTizenApplicationKey'
const char kTizenApplicationIdKey[] = "@id";
const char kTizenApplicationLaunchModeKey[] = "@launch_mode";
const char kTizenApplicationPackageKey[] = "@package";
const char kTizenApplicationRequiredVersionKey[] = "@required_version";

const char kTizenAppIdKey[] = "widget.application.@package";
const char kAllowNavigationKey[] = "widget.allow-navigation";
const char kCSPReportOnlyKey[] =
    "widget.content-security-policy-report-only.#text";
const char kTizenSettingKey[] = "widget.setting";
const char kTizenBackgroundSupportKey[] = "@background-support";
const char kTizenContextMenuKey[] = "@context-menu";
const char kTizenHardwareKey[] = "@hwkey-event";
const char kTizenEncryptionKey[] = "@encryption";
const char kTizenInstallLocationKey[] = "@install-location";
const char kTizenNoDisplayKey[] = "@nodisplay";
const char kTizenIndicatorPresenceKey[] = "@indicator-presence";
const char kTizenBackbuttonPresenceKey[] =
    "@backbutton-presence";
const char kTizenUserAgentKey[] = "@user-agent";
const char kTizenSoundModeKey[] = "@sound-mode";
const char kTizenBackgroundVibrationKey[] =
    "@background-vibration";
const char kTizenMetaDataKey[] = "widget.metadata";
// Child keys inside 'kTizenMetaDataKey'
const char kTizenMetaDataNameKey[] = "@key";
const char kTizenMetaDataValueKey[] = "@value";
const char kTizenPermissionsKey[] = "widget.privilege";
const char kTizenPermissionsNameKey[] = "@name";
const char kTizenSplashScreenKey[] = "widget.splash-screen";
const char kTizenSplashScreenSrcKey[] = "@src";
const char kContentNamespace[] = "widget.content.@namespace";
const char kTizenScreenOrientationKey[] = "@screen-orientation";
const char kTizenAppWidgetFullKey[] = "widget.app-widget";
const char kTizenAppWidgetKey[] = "app-widget";
const char kTizenAppWidgetIdKey[] = "@id";
const char kTizenAppWidgetPrimaryKey[] = "@primary";
const char kTizenAppWidgetUpdatePeriodKey[] = "@update-period";
const char kTizenAppWidgetAutoLaunchKey[] = "@auto-launch";
const char kTizenAppWidgetBoxLabelKey[] = "box-label";
const char kTizenAppWidgetBoxLabelLangKey[] = "@lang";
const char kTizenAppWidgetBoxLabelTextKey[] = "#text";
const char kTizenAppWidgetBoxIconKey[] = "box-icon";
const char kTizenAppWidgetBoxIconSrcKey[] = "@src";
const char kTizenAppWidgetBoxContentKey[] = "box-content";
const char kTizenAppWidgetBoxContentSrcKey[] = "@src";
const char kTizenAppWidgetBoxContentMouseEventKey[] = "@mouse-event";
const char kTizenAppWidgetBoxContentTouchEffectKey[] = "@touch-effect";
const char kTizenAppWidgetBoxContentSizeKey[] = "box-size";
const char kTizenAppWidgetBoxContentSizeTextKey[] = "#text";
const char kTizenAppWidgetBoxContentSizePreviewKey[] = "@preview";
const char kTizenAppWidgetBoxContentSizeUseDecorationKey[] = "@use-decoration";
const char kTizenAppWidgetBoxContentDropViewKey[] = "pd";
const char kTizenAppWidgetBoxContentDropViewSrcKey[] = "@src";
const char kTizenAppWidgetBoxContentDropViewWidthKey[] = "@width";
const char kTizenAppWidgetBoxContentDropViewHeightKey[] = "@height";
// App control
const char kTizenApplicationAppControlsKey[] = "widget.app-control";
const char kTizenApplicationAppControlSrcKey[] = "src";
const char kTizenApplicationAppControlOperationKey[] = "operation";
const char kTizenApplicationAppControlUriKey[] = "uri";
const char kTizenApplicationAppControlMimeKey[] = "mime";
const char kTizenApplicationAppControlOnResetKey[] = "on-reset";
const char kTizenApplicationAppControlChildNameAttrKey[] = "@name";
// IME
const char kTizenImeKey[] = "widget.ime";
const char kTizenImeUuidKey[] = "uuid";
const char kTizenImeUuidTextKey[] = "#text";
const char kTizenImeLanguagesKey[] = "languages";
const char kTizenImeLanguageKey[] = "language";
const char kTizenImeLanguageTextKey[] = "#text";
// Content
const char kTizenContentKey[] = "widget.content";
const char kTizenContentSrcKey[] = "@src";
const char kTizenContentEncodingKey[] = "@encoding";
const char kTizenContentTypeKey[] = "@type";
// Category
const char kTizenCategoryKey[] = "widget.category";
const char kTizenCategoryNameKey[] = "@name";
const char kTizenNamespacePrefix[] = "http://tizen.org/ns/widgets";
// Service
const char kTizenServiceKey[] = "widget.service";
const char kTizenServiceIdKey[] = "@id";
const char kTizenServiceAutoRestartKey[] = "@auto-restart";
const char kTizenServiceOnBootKey[] = "@on-boot";

const char kXmlTextKey[] = "#text";

}  // namespace application_widget_keys

}  // namespace wgt
