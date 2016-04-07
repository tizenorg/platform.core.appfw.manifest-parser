// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
#define TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


// Keys used in JSON representation of applications.
namespace tpk {
namespace application_keys {

// manifest
LIBSCL_EXPORT_API extern const char kManifestKey[];

// privileges
LIBSCL_EXPORT_API extern const char kPrivilegesKey[];

// service-application
LIBSCL_EXPORT_API extern const char kServiceApplicationKey[];

// ui-application
LIBSCL_EXPORT_API extern const char kUIApplicationKey[];

// widget-application
LIBSCL_EXPORT_API extern const char kWidgetApplicationKey[];

// watch-application
LIBSCL_EXPORT_API extern const char kWatchApplicationKey[];

// account
LIBSCL_EXPORT_API extern const char kAccountKey[];

// shortcuts
LIBSCL_EXPORT_API extern const char kShortcutListKey[];
LIBSCL_EXPORT_API extern const char kShortcutKey[];
LIBSCL_EXPORT_API extern const char kShortcutAppidKey[];
LIBSCL_EXPORT_API extern const char kShortcutExtraDataKey[];
LIBSCL_EXPORT_API extern const char kShortcutExtraKeyKey[];
LIBSCL_EXPORT_API extern const char kShortcutIconKey[];
LIBSCL_EXPORT_API extern const char kShortcutIconTextKey[];
LIBSCL_EXPORT_API extern const char kShortcutLabelKey[];
LIBSCL_EXPORT_API extern const char kShortcutLabelTextKey[];
LIBSCL_EXPORT_API extern const char kShortcutLabelLangKey[];

// profile
LIBSCL_EXPORT_API extern const char kProfileKey[];

}  // namespace application_keys
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
