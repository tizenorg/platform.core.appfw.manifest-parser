// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
#define TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_

// Keys used in JSON representation of applications.
namespace tpk {
namespace application_keys {

// manifest
extern const char kManifestKey[];

// author
extern const char kAuthorKey[];

// description
extern const char kDescriptionKey[];

// privileges
extern const char kPrivilegesKey[];

// service-application
extern const char kServiceApplicationKey[];

// ui-application
extern const char kUIApplicationKey[];
extern const char kUIApplicationLaunchModeKey[];

// account
extern const char kAccountKey[];

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
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
