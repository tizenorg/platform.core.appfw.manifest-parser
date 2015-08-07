// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {

namespace manifest_keys {

const char kManifestFileName[] = "tizen-manifest.xml";
const char kUIApplication[] = "ui-application";
const char kServiceApplication[] = "service-application";
const char kLable[] = "label";
const char kAPI[] = "api-version";
const char kPackage[] = "package";
const char kType[] = "tpk";
const char kVersion[] = "version";
const char kInstallLocation[] = "install-location";
const char kMainAppID[] = "appid";

}  // namespace manifest_keys

namespace application_keys {

// author
const char kAuthorKey[] = "author";
const char kAuthorKeyText[] = "#text";
const char kAuthorEmailKey[] = "email";
const char kAuthorEmailChildKey[] = "@email";
const char kAuthorHrefKey[] = "href";
const char kAuthorHrefChildKey[] = "@href";

// description
const char kDescriptionKey[] = "description";
const char kDescriptionKeyText[] = "#text";
const char kDescriptionLangKey[] = "xml:lang";

// privileges
const char kPrivilegesKey[] = "privileges";
// privilege
const char kPrivilegeKey[] = "privilege";

// service-application
const char kServiceApplicationKey[] = "service-application";
const char kServiceApplicationAppIDKey[] = "appid";
const char kServiceApplicationAutoRestartKey[] = "auto-restart";
const char kServiceApplicationExecKey[] = "exec";
const char kServiceApplicationOnBootKey[] = "on-boot";
const char kServiceApplicationTypeKey[] = "type";
const char kServiceApplicationKeyText[] = "#text";

// app-control
const char kAppControlKey[] = "app-control";
const char kAppControlOperationKey[] = "operation";
const char kAppControlNameKey[] = "name";

// datacontrol
const char kDataControlKey[] = "datacontrol";
const char kDataControlAccessKey[] = "access";
const char kDataControlProviderIDKey[] = "providerid";
const char kDataControlTypeKey[] = "type";
const char kDataControlTypeChildNameAttrKey[] = "@name";

// icon
const char kIconKey[] = "icon";

// label
const char kLableKey[] = "label";
const char kLableLangKey[] = "xml:lang";

// metadata
const char kMetaData[] = "metadata";
const char kMetaDataKey[] = "key";
const char kMetaDataValueKey[] = "value";
const char kMetaDataTypeChildNameAttrKey[] = "@name";

// ui-application
const char kUIApplicationKey[] = "ui-application";
const char kUIApplicationAppIDKey[] = "appid";
const char kUIApplicationExecKey[] = "exec";
const char kUIApplicationMultipleKey[] = "multiple";
const char kUIApplicationNoDisplayKey[] = "nodisplay";
const char kUIApplicationTaskManageKey[] = "taskmanage";
const char kUIApplicationTypeKey[] = "type";
const char kUIApplicationKeyText[] = "#text";

}  // namespace application_keys

namespace manifest_errors {

const char kNoManifestElement[] =
    "No mandatory manifest element in xml";
const char kNotUIorServiceElement[] =
    "Neither <ui-application> nor <service-application> "
    "element is found in manifest xml";
const char kNoLabelElement[] =
    "No mandatory <label> element in manifest xml";
const char kInvalidKey[] =
    "Value 'key' is missing or invalid.";
const char kInvalidName[] =
    "Required value 'name' is missing or invalid.";
const char kManifestParseError[] =
    "Manifest is not valid JSON.";
const char kManifestUnreadable[] =
    "Manifest file is missing or unreadable.";

}  // namespace manifest_errors
}  // namespace tpk
