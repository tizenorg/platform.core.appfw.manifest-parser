// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_MANIFEST_HANDLERS_TPK_CONFIG_PARSER_H_
#define TPK_MANIFEST_HANDLERS_TPK_CONFIG_PARSER_H_

#include <boost/filesystem/path.hpp>

#include <memory>
#include <string>

#include "manifest_parser/manifest_parser.h"

#ifndef LIBSCL_EXPORT_API
#define LIBSCL_EXPORT_API
#endif // LIBSCL_EXPORT_API


namespace tpk {
namespace parse {

/**
 * @brief The TPKConfigParser class
 *        Parser class of tpk package tizen-manifest.xml manifest file.
 *
 * Instance of this class may be used to parse single configuration file.
 * Depending on boolean result of @ref ParseManifest method, client code may
 * call:
 *  - on success -> @ref GetManifestData(), passing the key of ManifestData
 *                  instance that it is interested in.
 *  - on failure -> @ref GetErrorMessage(), to get value of error which was set
 *                  during the processing of config.xml
 *
 * To investigate which key do you need to get certain parsed piece of data,
 * check the key reported by handler's @ref ManifestHandler::Key() method.
 * Key returned by this method is the key to access data set by handler.
 */
class LIBSCL_EXPORT_API TPKConfigParser {
 public:
  TPKConfigParser();

  std::shared_ptr<const parser::ManifestData> GetManifestData(
      const std::string& key);
  const std::string& GetErrorMessage() const;
  bool ParseManifest(const boost::filesystem::path& path);

 private:
  std::unique_ptr<parser::ManifestParser> parser_;
  std::string error_;
};

}  // namespace parse
}  // namespace tpk

#endif  // TPK_MANIFEST_HANDLERS_TPK_CONFIG_PARSER_H_
