// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "manifest_parser/utils/iri_util.h"

#include <iri.h>

#include <memory>

namespace parser {
namespace utils {

bool IsValidIRI(const std::string& iri_string) {
  std::unique_ptr<iri_struct, decltype(&iri_destroy)> iri(
      iri_parse(iri_string.c_str()), iri_destroy);
  return
    iri != NULL &&
    iri->scheme != NULL && (
      iri->display != NULL ||
      iri->user != NULL ||
      iri->auth != NULL ||
      iri->password != NULL ||
      iri->host != NULL ||
      iri->path != NULL ||
      iri->query != NULL ||
      iri->anchor != NULL ||
      iri->qparams != NULL ||
      iri->schemelist != NULL);
}

}  // namespace utils
}  // namespace parser
