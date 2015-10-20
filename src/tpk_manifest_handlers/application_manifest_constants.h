// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
#define TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_

// Keys used in JSON representation of applications.
namespace tpk {
namespace manifest_keys {

extern const char kManifestKey[];
extern const char kUIApplication[];
extern const char kServiceApplication[];

}  // namespace manifest_keys

namespace application_keys {

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

// account
extern const char kAccountKey[];

}  // namespace application_keys
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
