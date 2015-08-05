// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
#define TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_

// Keys used in JSON representation of applications.
namespace tpk {
namespace manifest_keys {

extern const char kManifestFileName[];
extern const char kUIApplication[];
extern const char kServiceApplication[];
extern const char kLable[];
extern const char kAPI[];
extern const char kPackage[];
extern const char kType[];
extern const char kVersion[];
extern const char kInstallLocation[];
extern const char kMainAppID[];

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
// privilege
extern const char kPrivilegeKey[];

// service-application
extern const char kServiceApplicationKey[];
extern const char kServiceApplicationAppIDKey[];
extern const char kServiceApplicationAutoRestartKey[];
extern const char kServiceApplicationExecKey[];
extern const char kServiceApplicationOnBootKey[];
extern const char kServiceApplicationTypeKey[];

// app-control
extern const char kAppControlKey[];
extern const char kAppControlOperationKey[];
extern const char kAppControlNameKey[];

// datacontrol
extern const char kDataControlKey[];
extern const char kDataControlAccessKey[];
extern const char kDataControlProviderIDKey[];
extern const char kDataControlTypeKey[];
extern const char kDataControlTypeChildNameAttrKey[];

// icon
extern const char kIconKey[];

// label
extern const char kLableKey[];
extern const char kLableLangKey[];

// metadata
extern const char kMetadata[];
extern const char kMetadataKey[];
extern const char kMatadataValueKey[];

// ui-application
extern const char kUIApplicationKey[];
extern const char kUIApplicationAppIDKey[];
extern const char kUIApplicationExecKey[];
extern const char kUIApplicationMultipleKey[];
extern const char kUIApplicationNoDisplayKey[];
extern const char kUIApplicationTaskManageKey[];
extern const char kUIApplicationTypeKey[];

// datacontrol
extern const char kDataControlKey[];
extern const char kDataControlAccessKey[];
extern const char kDataControlProviderIDKey[];
extern const char kDataControlTypeKey[];

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
