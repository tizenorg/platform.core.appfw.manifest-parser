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

const char kAccessKey[] = "widget.access";
const char kLicenseKey[] = "widget.license";
const char kTizenNamespacePrefix[] = "http://tizen.org/ns/widgets";
const char kTizenWidgetKey[] = "widget";
const char kNamespaceKey[] = "@namespace";

const char kCSPKey[] = "widget.content-security-policy";
const char kCSPReportOnlyKey[] = "widget.content-security-policy-report-only";

// Child keys inside 'kPreferencesKey'.
const char kPreferencesNameKey[] = "@name";
const char kPreferencesValueKey[] = "@value";
const char kPreferencesReadonlyKey[] = "@readonly";

const char kWidgetLangKey[] = "widget.@lang";
const char kWidgetNamespaceKey[] = "widget.@namespace";
const char kWidgetNamespacePrefix[] = "http://www.w3.org/ns/widgets";

const char kTizenApplicationKey[] = "widget.application";
// Child keys inside 'kTizenApplicationKey'
const char kTizenApplicationIdKey[] = "@id";
const char kTizenApplicationLaunchModeKey[] = "@launch_mode";
const char kTizenApplicationPackageKey[] = "@package";
const char kTizenApplicationRequiredVersionKey[] = "@required_version";

const char kTizenAppIdKey[] = "widget.application.@package";
const char kAllowNavigationKey[] = "widget.allow-navigation";
const char kTizenSettingKey[] = "widget.setting";

const char kTizenInstallLocationKey[] = "@install-location";

const char kTizenUserAgentKey[] = "@user-agent";
const char kTizenSoundModeKey[] = "@sound-mode";
const char kTizenBackgroundVibrationKey[] =
    "@background-vibration";
const char kTizenMetaDataKey[] = "widget.metadata";
// Child keys inside 'kTizenMetaDataKey'
const char kTizenPermissionsKey[] = "widget.privilege";
const char kTizenSplashScreenKey[] = "widget.splash-screen";
const char kContentNamespace[] = "widget.content.@namespace";
const char kTizenAppWidgetFullKey[] = "widget.app-widget";

// App control
const char kTizenApplicationAppControlsKey[] = "widget.app-control";

// IME
const char kTizenImeKey[] = "widget.ime";
// Content
const char kTizenContentKey[] = "widget.content";
const char kTizenContentSrcKey[] = "@src";
// Category
const char kTizenCategoryKey[] = "widget.category";
// Service
const char kTizenServiceKey[] = "widget.service";
const char kTizenServiceIdKey[] = "@id";
const char kTizenServiceAutoRestartKey[] = "@auto-restart";
const char kTizenServiceOnBootKey[] = "@on-boot";

const char kXmlTextKey[] = "#text";

}  // namespace application_widget_keys

}  // namespace wgt
