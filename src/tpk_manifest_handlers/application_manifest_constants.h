// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
#define TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_

// Keys used in JSON representation of applications.
namespace tpk {
namespace manifest_keys {

extern const char kManifestKey[];
extern const char kManifestFileName[];
extern const char kUIApplication[];
extern const char kServiceApplication[];
extern const char kNamespace[];
extern const char kLabel[];
extern const char kAPI[];
extern const char kPackage[];
extern const char kType[];
extern const char kVersion[];
extern const char kInstallLocation[];
extern const char kMainAppID[];
extern const char kManifestTextKey[];

}  // namespace manifest_keys

namespace application_keys {

// author
extern const char kAuthorKey[];
extern const char kAuthorKeyText[];
extern const char kAuthorEmailKey[];
extern const char kAuthorEmailChildKey[];
extern const char kAuthorHrefKey[];
extern const char kAuthorHrefChildKey[];

// description
extern const char kDescriptionKey[];
extern const char kDescriptionKeyText[];
extern const char kDescriptionLangKey[];

// privileges
extern const char kPrivilegesKey[];
extern const char kPrivilegeKey[];
extern const char kPrivilegeTextKey[];

// service-application
extern const char kServiceApplicationKey[];
extern const char kServiceApplicationAppIDKey[];
extern const char kServiceApplicationAutoRestartKey[];
extern const char kServiceApplicationExecKey[];
extern const char kServiceApplicationOnBootKey[];
extern const char kServiceApplicationTypeKey[];
extern const char kServiceApplicationKeyText[];

// app-control
extern const char kAppControlKey[];
extern const char kAppControlOperationKey[];
extern const char kAppControlURIKey[];
extern const char kAppControlMimeKey[];
extern const char kAppControlNameChildKey[];

// datacontrol
extern const char kDataControlKey[];
extern const char kDataControlAccessKey[];
extern const char kDataControlProviderIDKey[];
extern const char kDataControlTypeKey[];

// icon
extern const char kIconKey[];
extern const char kIconTextKey[];

// label
extern const char kLabelKey[];
extern const char kLabelKeyText[];
extern const char kLabelLangKey[];

// metadata
extern const char kMetaData[];
extern const char kMetaDataKey[];
extern const char kMetaDataValueKey[];

// ui-application
extern const char kUIApplicationKey[];
extern const char kUIApplicationAppIDKey[];
extern const char kUIApplicationExecKey[];
extern const char kUIApplicationLaunchModeKey[];
extern const char kUIApplicationMultipleKey[];
extern const char kUIApplicationNoDisplayKey[];
extern const char kUIApplicationTaskManageKey[];
extern const char kUIApplicationTypeKey[];
extern const char kUIApplicationKeyText[];

// datacontrol
extern const char kDataControlKey[];
extern const char kDataControlAccessKey[];
extern const char kDataControlProviderIDKey[];
extern const char kDataControlTypeKey[];

// account
extern const char kAccountKey[];
extern const char kAccountProviderKey[];
extern const char kAccountProviderIDKey[];
extern const char kAccountAppIDKey[];
extern const char kAccountMASKey[];
extern const char kAccountIconKey[];
extern const char kAccountSectionKey[];
extern const char kAccountTextKey[];
extern const char kAccountLabelKey[];
extern const char kAccountCapabilityKey[];
extern const char kAccountLangKey[];
extern const char kAccountIconNormalKey[];
extern const char kAccountIconSmallKey[];

// shortcuts
extern const char kShortcutListKey[];
extern const char kShortcutKey[];
extern const char kShortcutAppidKey[];
extern const char kShortcutExtraDataKey[];
extern const char kShortcutExtraKeyKey[];
extern const char kShortcutIconKey[];
extern const char kShortcutIconTextKey[];
extern const char kShortcutLabelKey[];
extern const char kShortcutLabelTextKey[];
extern const char kShortcutLabelLangKey[];

}  // namespace application_keys

namespace manifest_errors {

extern const char kNoManifestElement[];
extern const char kNotUIorServiceElement[];
extern const char kNoLabelElement[];
extern const char kInvalidKey[];
extern const char kInvalidName[];
extern const char kManifestParseError[];
extern const char kManifestUnreadable[];

}  // namespace manifest_errors
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
