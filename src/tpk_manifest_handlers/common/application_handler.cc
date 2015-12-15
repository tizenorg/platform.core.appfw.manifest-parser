// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk_manifest_handlers/common/application_handler.h"

namespace tpk {
namespace parse {

void ApplicationInfo::set_appid(const std::string& appid) {
  appid_ = appid;
}

void ApplicationInfo::set_exec(const std::string& exec) {
  exec_ = exec;
}

void ApplicationInfo::set_type(const std::string& type) {
  type_ = type;
}
void ApplicationInfo::set_process_pool(const std::string& process_pool) {
  process_pool_ = process_pool;
}

const std::string& ApplicationInfo::appid() const {
  return appid_;
}

const std::string& ApplicationInfo::exec() const {
  return exec_;
}

const std::string& ApplicationInfo::type() const {
  return type_;
}
const std::string& ApplicationInfo::process_pool() const {
  return process_pool_;
}

}  // parse
}  // tpk
