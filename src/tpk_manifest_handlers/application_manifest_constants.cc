// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/application_manifest_constants.h"

namespace tpk {

namespace manifest_keys {

const char kManifestKey[] = "manifest";
const char kManifestFileName[] = "tizen-manifest.xml";
const char kUIApplication[] = "ui-application";
const char kServiceApplication[] = "service-application";
const char kXMLS[] = "xmlns";
const char kLable[] = "label";
const char kAPI[] = "api-version";
const char kPackage[] = "package";
const char kType[] = "tpk";
const char kVersion[] = "version";
const char kInstallLocation[] = "install-location";
const char kMainAppID[] = "appid";
const char kManifestTextKey[] = "#text";

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
const char kPrivilegeKey[] = "privilege";
const char kPrivilegeTextKey[] = "#text";

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
const char kAppControlURIKey[] = "uri";
const char kAppControlMimeKey[] = "mime";
const char kAppControlNameChildKey[] = "@name";

// datacontrol
const char kDataControlKey[] = "datacontrol";
const char kDataControlAccessKey[] = "access";
const char kDataControlProviderIDKey[] = "providerid";
const char kDataControlTypeKey[] = "type";
const char kDataControlTypeChildNameAttrKey[] = "@name";

// icon
const char kIconKey[] = "icon";
const char kIconsKey[] = "icons";
const char kIconSrcKey[] = "@src";

// label
const char kLableKey[] = "label";
const char kLableKeyText[] = "#text";
const char kLableKeyName[] = "@name";
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

// account
const char kAccountKey[] = "account";
const char kAccountProviderKey[] = "account-provider";
const char kAccountProviderIDKey[] = "providerid";
const char kAccountAppIDKey[] = "appid";
const char kAccountMASKey[] = "multiple-account-support";
const char kAccountIconKey[] = "icon";
const char kAccountSectionKey[] = "section";
const char kAccountTextKey[] = "#text";
const char kAccountNameKey[] = "@name";
const char kAccountLabelKey[] = "label";
const char kAccountCapabilityKey[] = "capability";
const char kAccountLangKey[] = "xml:lang";
const char kAccountIconNormalKey[] = "account";
const char kAccountIconSmallKey[] = "account-small";

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
