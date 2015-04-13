// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_handlers/application_manifest_constants.h"

namespace wgt {

namespace application_manifest_keys {

// Official fields (ordered as spec):

const char kNameKey[] = "name";
const char kDisplay[] = "display";
const char kStartURLKey[] = "start_url";
const char kCSPKey[] = "csp";

const char kIconsKey[] = "icons";

// Deprecated entries:

const char kAppKey[] = "app";
const char kLaunchLocalPathKey[] = "app.launch.local_path";
const char kLaunchWebURLKey[] = "app.launch.web_url";
const char kDeprecatedURLKey[] = "url";
const char kDeprecatedVersionKey[] = "version";
const char kDeprecatedDescriptionKey[] = "description";
const char kDeprecatedCSPKey[] = "content_security_policy";
const char kLaunchScreen[] = "launch_screen";
const char kLaunchScreenDefault[] = "launch_screen.default";
const char kLaunchScreenImageBorderDefault[] =
    "launch_screen.default.image_border";
const char kLaunchScreenImageBorderLandscape[] =
    "launch_screen.landscape.image_border";
const char kLaunchScreenImageBorderPortrait[] =
    "launch_screen.portrait.image_border";
const char kLaunchScreenLandscape[] =
    "launch_screen.landscape";
const char kLaunchScreenPortrait[] =
    "launch_screen.portrait";
const char kLaunchScreenReadyWhen[] =
    "launch_screen.ready_when";

}  // namespace application_manifest_keys

// manifest keys for widget applications.
namespace application_widget_keys {

const char kNamespaceKey[] = "@namespace";
const char kXmlLangKey[] = "@lang";
const char kDefaultLocaleKey[] = "widget.@defaultlocale";
const char kNameKey[] = "widget.name.#text";
const char kVersionKey[] = "widget.@version";
const char kViewModesKey[] = "widget.@viewmodes";
const char kWidgetKey[] = "widget";
const char kLaunchLocalPathKey[] = "widget.content.@src";
const char kWebURLsKey[] = "widget.@id";
const char kAuthorKey[] = "widget.author.#text";
const char kDescriptionKey[] = "widget.description.#text";
const char kShortNameKey[] = "widget.name.@short";
const char kIDKey[] = "widget.@id";
const char kAuthorEmailKey[] = "widget.author.@email";
const char kAuthorHrefKey[] = "widget.author.@href";
const char kHeightKey[] = "widget.@height";
const char kWidthKey[] = "widget.@width";
const char kPreferencesKey[] = "widget.preference";
const char kCSPKey[] = "widget.content-security-policy.#text";
const char kAccessKey[] = "widget.access";

// Child keys inside 'kPreferencesKey'.
const char kPreferencesNameKey[] = "@name";
const char kPreferencesValueKey[] = "@value";
const char kPreferencesReadonlyKey[] = "@readonly";

const char kWidgetNamespaceKey[] = "widget.@namespace";
const char kWidgetNamespacePrefix[] = "http://www.w3.org/ns/widgets";

// Child keys inside 'kAccessKey'.
const char kAccessOriginKey[] = "@origin";
const char kAccessSubdomainsKey[] = "@subdomains";

const char kTizenWidgetKey[] = "widget";
const char kIcon128Key[] = "widget.icon.@src";
const char kTizenApplicationKey[] = "widget.application";
// Child keys inside 'kTizenApplicationKey'
const char kTizenApplicationIdKey[] = "@id";
const char kTizenApplicationPackageKey[] = "@package";
const char kTizenApplicationRequiredVersionKey[] = "@required_version";

const char kTizenAppIdKey[] = "widget.application.@package";
const char kAllowNavigationKey[] = "widget.allow-navigation.#text";
const char kCSPReportOnlyKey[] =
    "widget.content-security-policy-report-only.#text";
const char kTizenSettingKey[] = "widget.setting";
const char kTizenBackgroundSupportKey[] = "widget.setting.@background-support";
const char kTizenContextMenuKey[] = "widget.setting.@context-menu";
const char kTizenHardwareKey[] = "widget.setting.@hwkey-event";
const char kTizenEncryptionKey[] = "widget.setting.@encryption";
const char kTizenMetaDataKey[] = "widget.metadata";
// Child keys inside 'kTizenMetaDataKey'
const char kTizenMetaDataNameKey[] = "@key";
const char kTizenMetaDataValueKey[] = "@value";
const char kTizenPermissionsKey[] = "widget.privilege";
const char kTizenPermissionsNameKey[] = "@name";
const char kTizenSplashScreenKey[] = "widget.splash-screen";
const char kTizenSplashScreenSrcKey[] = "@src";
const char kContentNamespace[] = "widget.content.@namespace";
const char kTizenScreenOrientationKey[] = "widget.setting.@screen-orientation";
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
const char kTizenApplicationAppControlChildNameAttrKey[] = "@name";
// IME
const char kTizenImeKey[] = "widget.ime";
const char kTizenImeUuidKey[] = "uuid";
const char kTizenImeUuidTextKey[] = "#text";
const char kTizenImeLanguagesKey[] = "languages";
const char kTizenImeLanguageKey[] = "language";
const char kTizenImeLanguageTextKey[] = "#text";
// category
const char kTizenCategoryKey[] = "widget.category";
const char kTizenCategoryNameKey[] = "@name";
const char kTizenNamespacePrefix[] = "http://tizen.org/ns/widgets";
// service
const char kTizenServiceKey[] = "widget.service";
const char kTizenServiceIdKey[] = "@id";
const char kTizenServiceAutoRestartKey[] = "@auto-restart";
const char kTizenServiceOnBootKey[] = "@on-boot";

}  // namespace application_widget_keys

namespace application_manifest_errors {
const char kInvalidDescription[] =
    "Invalid value for 'description'.";
const char kInvalidKey[] =
    "Value 'key' is missing or invalid.";
const char kInvalidName[] =
    "Required value 'name' is missing or invalid.";
const char kManifestParseError[] =
    "Manifest is not valid JSON.";
const char kManifestUnreadable[] =
    "Manifest file is missing or unreadable.";
}  // namespace application_manifest_errors
}  // namespace wgt
