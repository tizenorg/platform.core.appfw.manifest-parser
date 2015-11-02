// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#ifndef MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
#define MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_

#include "manifest_parser/manifest.h"

// Keys used in JSON representation of applications.
namespace wgt {
namespace application_manifest_keys {

// TODO(p.sikorski): below key should be removed, when nwrt and app-installer
// will stop use it. Now, it is just copied.
extern const char kIconsKey[];

}  // namespace application_manifest_keys

}  // namespace wgt

#endif  // MANIFEST_HANDLERS_APPLICATION_MANIFEST_CONSTANTS_H_
