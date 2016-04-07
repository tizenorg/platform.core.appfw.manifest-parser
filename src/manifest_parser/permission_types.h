// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_PARSER_PERMISSION_TYPES_H_
#define MANIFEST_PARSER_PERMISSION_TYPES_H_

#include <map>
#include <set>
#include <string>

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace parser {

enum LIBSCL_EXPORT_API PermissionType {
  SESSION_PERMISSION,
  PERSISTENT_PERMISSION,
};

enum LIBSCL_EXPORT_API RuntimePermission {
  ALLOW_ONCE = 0,
  ALLOW_SESSION,
  ALLOW_ALWAYS,
  DENY_ONCE,
  DENY_SESSION,
  DENY_ALWAYS,
  UNDEFINED_RUNTIME_PERM,
};

enum LIBSCL_EXPORT_API StoredPermission {
  ALLOW = 0,
  DENY,
  PROMPT,
  UNDEFINED_STORED_PERM,
};

typedef std::map<std::string, StoredPermission> StoredPermissionMap;
typedef std::set<std::string> PermissionSet;
typedef std::set<std::string> PrivilegesSet;

}  // namespace parser

#endif  // MANIFEST_PARSER_PERMISSION_TYPES_H_
