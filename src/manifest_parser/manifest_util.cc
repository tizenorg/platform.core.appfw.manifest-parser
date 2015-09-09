// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE-xwalk file.

#include "manifest_parser/manifest_util.h"

#include <boost/filesystem/operations.hpp>

#include <algorithm>
#include <cassert>
#include <map>
#include <regex>
#include <vector>

#include "utils/string_util.h"
#include "manifest_parser/manifest.h"
#include "manifest_parser/manifest_handler.h"
#include "manifest_parser/values.h"

namespace bf = boost::filesystem;

namespace {
const char kAttributePrefix[] = "@";

const xmlChar kWidgetNodeKey[] = "widget";
const xmlChar kNameNodeKey[] = "name";
const xmlChar kDescriptionNodeKey[] = "description";
const xmlChar kAuthorNodeKey[] = "author";
const xmlChar kLicenseNodeKey[] = "license";
const xmlChar kIconNodeKey[] = "icon";
const xmlChar kPreferenceNodeKey[] = "preference";
const xmlChar kContentNodeKey[] = "content";

const xmlChar kVersionAttributeKey[] = "version";
const xmlChar kShortAttributeKey[] = "short";
const xmlChar kDirAttributeKey[] = "dir";
const xmlChar kEmailAttributeKey[] = "email";
const xmlChar kHrefAttributeKey[] = "href";
const xmlChar kIdAttributeKey[] = "id";
const xmlChar kDefaultLocaleAttributeKey[] = "defaultlocale";
const xmlChar kPathAttributeKey[] = "path";
const xmlChar kNameAttributeKey[] = "name";
const xmlChar kValueAttributeKey[] = "value";
const xmlChar kSrcAttributeKey[] = "src";

const char kXmlTextKey[] = "#text";
const char kNamespaceKey[] = "@namespace";

const char kIdPattern[] = "^[0-9a-zA-Z]{10}[.][0-9a-zA-Z]{1,52}$";
const char kPackagePattern[] = "^[0-9a-zA-Z]{10}$";

}  // namespace

namespace parser {

std::string GetNodeDir(xmlNode* node, const std::string& inherit_dir) {
  assert(node);
  std::string dir(inherit_dir);

  xmlAttr* prop = nullptr;
  for (prop = node->properties; prop; prop = prop->next) {
    if (xmlStrEqual(prop->name, kDirAttributeKey)) {
      char* prop_value = reinterpret_cast<char *>(xmlNodeListGetString(
          node->doc, prop->children, 1));
      dir = prop_value;
      xmlFree(prop_value);
      break;
    }
  }
  return dir;
}

std::string GetNodeText(xmlNode* root, const std::string& inherit_dir) {
  assert(root);
  if (root->type != XML_ELEMENT_NODE)
    return std::string();
  std::string current_dir(GetNodeDir(root, inherit_dir));
  std::string text;
  if (!current_dir.empty())
    text += parser::utils::GetDirUTF8Start(current_dir);
  for (xmlNode* node = root->children; node; node = node->next) {
    if (node->type == XML_TEXT_NODE || node->type == XML_CDATA_SECTION_NODE)
      text = text + std::string(reinterpret_cast<char*>(node->content));
    else
      text += GetNodeText(node, current_dir);
  }
  if (!current_dir.empty())
    text += parser::utils::GetDirUTF8End();
  return text;
}

// According to widget specification, this two prop need to support dir.
// see detail on http://www.w3.org/TR/widgets/#the-dir-attribute
bool IsPropSupportDir(xmlNode* root, xmlAttr* prop) {
  if (xmlStrEqual(root->name, kWidgetNodeKey)
     && xmlStrEqual(prop->name, kVersionAttributeKey))
    return true;
  if (xmlStrEqual(root->name, kNameNodeKey)
     && xmlStrEqual(prop->name, kShortAttributeKey))
    return true;
  return false;
}

// Only this four items need to support span and ignore other element.
// Besides xmlNodeListGetString can not support dir prop of span.
// See http://www.w3.org/TR/widgets/#the-span-element-and-its-attributes
bool IsElementSupportSpanAndDir(xmlNode* root) {
  if (xmlStrEqual(root->name, kNameNodeKey)
     || xmlStrEqual(root->name, kDescriptionNodeKey)
     || xmlStrEqual(root->name, kAuthorNodeKey)
     || xmlStrEqual(root->name, kLicenseNodeKey))
    return true;
  return false;
}

// According to spec 'name' and 'author' should be result of applying the rule
// for getting text content with normalized white space to this element.
// http://www.w3.org/TR/widgets/#rule-for-getting-text-content-with-normalized-white-space-0
bool IsTrimRequiredForElement(xmlNode* root) {
  if (xmlStrEqual(root->name, kNameNodeKey) ||
      xmlStrEqual(root->name, kAuthorNodeKey)) {
    return true;
  }
  return false;
}

// According to spec some attributes requaire applying the rule for getting
// a single attribute value.
// http://www.w3.org/TR/widgets/#rule-for-getting-a-single-attribute-value-0
bool IsTrimRequiredForProp(xmlNode* root, xmlAttr* prop) {
  if (xmlStrEqual(root->name, kWidgetNodeKey) &&
      (xmlStrEqual(prop->name, kIdAttributeKey) ||
      xmlStrEqual(prop->name, kVersionAttributeKey) ||
      xmlStrEqual(prop->name, kDefaultLocaleAttributeKey))) {
    return true;
  }
  if (xmlStrEqual(root->name, kNameNodeKey) &&
      xmlStrEqual(prop->name, kShortAttributeKey)) {
    return true;
  }
  if (xmlStrEqual(root->name, kAuthorNodeKey) &&
      (xmlStrEqual(prop->name, kEmailAttributeKey) ||
      xmlStrEqual(prop->name, kHrefAttributeKey))) {
    return true;
  }
  if (xmlStrEqual(root->name, kLicenseNodeKey) &&
      xmlStrEqual(prop->name, kHrefAttributeKey)) {
    return true;
  }
  if (xmlStrEqual(root->name, kIconNodeKey) &&
      xmlStrEqual(prop->name, kPathAttributeKey)) {
    return true;
  }
  if (xmlStrEqual(root->name, kPreferenceNodeKey) &&
      (xmlStrEqual(prop->name, kNameAttributeKey) ||
      xmlStrEqual(prop->name, kValueAttributeKey))) {
    return true;
  }
  if (xmlStrEqual(root->name, kContentNodeKey) &&
      xmlStrEqual(prop->name, kSrcAttributeKey)) {
    return true;
  }
  return false;
}

// Load XML node into Dictionary structure.
// The keys for the XML node to Dictionary mapping are described below:
// XML                                 Dictionary
// <e></e>                             "e":{"#text": ""}
// <e>textA</e>                        "e":{"#text":"textA"}
// <e attr="val">textA</e>             "e":{ "@attr":"val", "#text": "textA"}
// <e> <a>textA</a> <b>textB</b> </e>  "e":{
//                                       "a":{"#text":"textA"}
//                                       "b":{"#text":"textB"}
//                                     }
// <e> <a>textX</a> <a>textY</a> </e>  "e":{
//                                       "a":[ {"#text":"textX"},
//                                             {"#text":"textY"}]
//                                     }
// <e> textX <a>textY</a> </e>         "e":{ "#text":"textX",
//                                           "a":{"#text":"textY"}
//                                     }
//
// For elements that are specified under a namespace, the dictionary
// will add '@namespace' key for them, e.g.,
// XML:
// <e xmln="linkA" xmlns:N="LinkB">
//   <sub-e1> text1 </sub-e>
//   <N:sub-e2 text2 />
// </e>
// will be saved in Dictionary as,
// "e":{
//   "#text": "",
//   "@namespace": "linkA"
//   "sub-e1": {
//     "#text": "text1",
//     "@namespace": "linkA"
//   },
//   "sub-e2": {
//     "#text":"text2"
//     "@namespace": "linkB"
//   }
// }

// converting dictionaryvalue to std::map<
// std::string*, std::map<std::string*,std::string*>> or
// std::map<std::string*, std::map<std::string*,std::string>>

std::unique_ptr<DictionaryValue> LoadXMLNode(
    xmlNode* root, const std::string& inherit_dir) {
  std::unique_ptr<DictionaryValue> value(new DictionaryValue());
  if (root->type != XML_ELEMENT_NODE)
    return nullptr;

  std::string current_dir(GetNodeDir(root, inherit_dir));

  xmlAttr* prop = nullptr;
  for (prop = root->properties; prop; prop = prop->next) {
    xmlChar* value_ptr = xmlNodeListGetString(root->doc, prop->children, 1);
    std::string prop_value(reinterpret_cast<char*>(value_ptr));
    xmlFree(value_ptr);

    if (IsPropSupportDir(root, prop))
      prop_value = utils::GetDirTextUTF8(prop_value, current_dir);

    if (IsTrimRequiredForProp(root, prop))
      prop_value = utils::CollapseWhitespaceUTF8(prop_value);

    value->SetString(
        std::string(kAttributePrefix)
        + reinterpret_cast<const char*>(prop->name),
        prop_value);
  }

  if (root->ns)
    value->SetString(kNamespaceKey,
        reinterpret_cast<const char*>(root->ns->href));

  for (xmlNode* node = root->children; node; node = node->next) {
    std::string sub_node_name(reinterpret_cast<const char*>(node->name));
    std::unique_ptr<DictionaryValue> sub_value =
        LoadXMLNode(node, current_dir);
    if (!sub_value)
      continue;

    if (!value->HasKey(sub_node_name)) {
      value->Set(sub_node_name, sub_value.release());
      continue;
    }

    Value* temp;
    value->Get(sub_node_name, &temp);
    assert(temp);

    if (temp->IsType(Value::TYPE_LIST)) {
      ListValue* list;
      temp->GetAsList(&list);
      list->Append(sub_value.release());
    } else {
      assert(temp->IsType(Value::TYPE_DICTIONARY));
      DictionaryValue* dict;
      temp->GetAsDictionary(&dict);
      DictionaryValue* prev_value = dict->DeepCopy();

      ListValue* list = new ListValue();
      list->Append(prev_value);
      list->Append(sub_value.release());
      value->Set(sub_node_name, list);
    }
  }

  std::string text;
  if (IsElementSupportSpanAndDir(root)) {
    text = GetNodeText(root, current_dir);
  } else {
    xmlChar* text_ptr = xmlNodeListGetString(root->doc, root->children, 1);
    if (text_ptr) {
      text = reinterpret_cast<char*>(text_ptr);
      xmlFree(text_ptr);
    }
  }

  if (IsTrimRequiredForElement(root))
    text = utils::CollapseWhitespaceUTF8(text);

  if (!text.empty())
    value->SetString(kXmlTextKey, text);

  return value;
}

std::shared_ptr<Manifest> LoadManifest(const std::string& manifest_path,
                                       std::string* error) {
  xmlDoc * doc = nullptr;
  xmlNode* root_node = nullptr;
  doc = xmlReadFile(manifest_path.c_str(), nullptr, XML_PARSE_NOENT);
  if (!doc) {
    *error = "Manifest file is missing or unreadable.";
    return nullptr;
  }
  root_node = xmlDocGetRootElement(doc);
  std::unique_ptr<DictionaryValue> dv = LoadXMLNode(root_node);
  std::unique_ptr<DictionaryValue> result(new DictionaryValue);
  if (dv)
    result->Set(reinterpret_cast<const char*>(root_node->name), dv.release());
  return std::make_shared<parser::Manifest>(std::move(result));
}

bf::path ApplicationURLToRelativeFilePath(const std::string& url) {
  std::string url_path = url;
  if (url_path.empty() || url_path[0] != '/')
    return bf::path();

  std::string file_path = utils::DecodePercentEscapedCharacter(url_path);
  if (file_path.empty())
    return bf::path();

  bf::path path(file_path);

  // It's still possible for someone to construct an annoying URL whose path
  // would still wind up not being considered relative at this point.
  // For example: app://id/c:////foo.html
  if (path.is_absolute())
    return bf::path();

  return path;
}

bool ValidateTizenApplicationId(const std::string& id) {
  std::regex id_regex(kIdPattern);
  return std::regex_match(id, id_regex);
}

bool ValidateTizenPackageId(const std::string& id) {
  std::regex package_regex(kPackagePattern);
  return std::regex_match(id, package_regex);
}

}  // namespace parser
