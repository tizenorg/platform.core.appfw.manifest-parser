// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "manifest_parser/utils/w3c_languages.h"

#include <boost/filesystem.hpp>
#include <tzplatform_config.h>
#include <algorithm>
#include <fstream>
#include <vector>

#include "manifest_parser/utils/logger.h"

namespace bf = boost::filesystem;
namespace {
const bf::path kLanguagesFile =
    bf::path(SHAREDIR)
        / "manifest-parser/languages-only.txt";
const int kLanguagesNumber = 8094;
}

namespace utils {
namespace w3c_languages {
namespace lang_set {

std::vector<std::string> w3c_languages;

bool InitializeLanguageSet() {
  if (!w3c_languages.empty())
    return true;
  if (!bf::exists(kLanguagesFile)) {
    LOG(ERROR) << "File does not exist";
    return false;
  }
  std::ifstream infile(kLanguagesFile.string());
  if (!infile.is_open()) {
    LOG(ERROR) << "Error during file opening";
    return false;
  }
  w3c_languages.reserve(kLanguagesNumber);
  std::string line;
  while (std::getline(infile, line)) {
    w3c_languages.push_back(line);
  }
  infile.close();
  return true;
}

bool ValidateOnlyLanguage(const std::string& language) {
  if (!InitializeLanguageSet())
    return false;
  if (!std::binary_search(w3c_languages.begin(), w3c_languages.end(), language))
    return false;
  return true;
}
}  // namespace lang_set

const std::map<std::string, std::string> kW3CExtLang{{"aao", "ar"},
                                                     {"abh", "ar"},
                                                     {"abv", "ar"},
                                                     {"acm", "ar"},
                                                     {"acq", "ar"},
                                                     {"acw", "ar"},
                                                     {"acx", "ar"},
                                                     {"acy", "ar"},
                                                     {"adf", "ar"},
                                                     {"ads", "sgn"},
                                                     {"aeb", "ar"},
                                                     {"aec", "ar"},
                                                     {"aed", "sgn"},
                                                     {"aen", "sgn"},
                                                     {"afb", "ar"},
                                                     {"afg", "sgn"},
                                                     {"ajp", "ar"},
                                                     {"apc", "ar"},
                                                     {"apd", "ar"},
                                                     {"arb", "ar"},
                                                     {"arq", "ar"},
                                                     {"ars", "ar"},
                                                     {"ary", "ar"},
                                                     {"arz", "ar"},
                                                     {"ase", "sgn"},
                                                     {"asf", "sgn"},
                                                     {"asp", "sgn"},
                                                     {"asq", "sgn"},
                                                     {"asw", "sgn"},
                                                     {"auz", "ar"},
                                                     {"avl", "ar"},
                                                     {"ayh", "ar"},
                                                     {"ayl", "ar"},
                                                     {"ayn", "ar"},
                                                     {"ayp", "ar"},
                                                     {"bbz", "ar"},
                                                     {"bfi", "sgn"},
                                                     {"bfk", "sgn"},
                                                     {"bjn", "ms"},
                                                     {"bog", "sgn"},
                                                     {"bqn", "sgn"},
                                                     {"bqy", "sgn"},
                                                     {"btj", "ms"},
                                                     {"bve", "ms"},
                                                     {"bvl", "sgn"},
                                                     {"bvu", "ms"},
                                                     {"bzs", "sgn"},
                                                     {"cdo", "zh"},
                                                     {"cds", "sgn"},
                                                     {"cjy", "zh"},
                                                     {"cmn", "zh"},
                                                     {"coa", "ms"},
                                                     {"cpx", "zh"},
                                                     {"csc", "sgn"},
                                                     {"csd", "sgn"},
                                                     {"cse", "sgn"},
                                                     {"csf", "sgn"},
                                                     {"csg", "sgn"},
                                                     {"csl", "sgn"},
                                                     {"csn", "sgn"},
                                                     {"csq", "sgn"},
                                                     {"csr", "sgn"},
                                                     {"czh", "zh"},
                                                     {"czo", "zh"},
                                                     {"doq", "sgn"},
                                                     {"dse", "sgn"},
                                                     {"dsl", "sgn"},
                                                     {"dup", "ms"},
                                                     {"ecs", "sgn"},
                                                     {"esl", "sgn"},
                                                     {"esn", "sgn"},
                                                     {"eso", "sgn"},
                                                     {"eth", "sgn"},
                                                     {"fcs", "sgn"},
                                                     {"fse", "sgn"},
                                                     {"fsl", "sgn"},
                                                     {"fss", "sgn"},
                                                     {"gan", "zh"},
                                                     {"gds", "sgn"},
                                                     {"gom", "kok"},
                                                     {"gse", "sgn"},
                                                     {"gsg", "sgn"},
                                                     {"gsm", "sgn"},
                                                     {"gss", "sgn"},
                                                     {"gus", "sgn"},
                                                     {"hab", "sgn"},
                                                     {"haf", "sgn"},
                                                     {"hak", "zh"},
                                                     {"hds", "sgn"},
                                                     {"hji", "ms"},
                                                     {"hks", "sgn"},
                                                     {"hos", "sgn"},
                                                     {"hps", "sgn"},
                                                     {"hsh", "sgn"},
                                                     {"hsl", "sgn"},
                                                     {"hsn", "zh"},
                                                     {"icl", "sgn"},
                                                     {"iks", "sgn"},
                                                     {"ils", "sgn"},
                                                     {"inl", "sgn"},
                                                     {"ins", "sgn"},
                                                     {"ise", "sgn"},
                                                     {"isg", "sgn"},
                                                     {"isr", "sgn"},
                                                     {"jak", "ms"},
                                                     {"jax", "ms"},
                                                     {"jcs", "sgn"},
                                                     {"jhs", "sgn"},
                                                     {"jls", "sgn"},
                                                     {"jos", "sgn"},
                                                     {"jsl", "sgn"},
                                                     {"jus", "sgn"},
                                                     {"kgi", "sgn"},
                                                     {"knn", "kok"},
                                                     {"kvb", "ms"},
                                                     {"kvk", "sgn"},
                                                     {"kvr", "ms"},
                                                     {"kxd", "ms"},
                                                     {"lbs", "sgn"},
                                                     {"lce", "ms"},
                                                     {"lcf", "ms"},
                                                     {"liw", "ms"},
                                                     {"lls", "sgn"},
                                                     {"lsg", "sgn"},
                                                     {"lsl", "sgn"},
                                                     {"lso", "sgn"},
                                                     {"lsp", "sgn"},
                                                     {"lst", "sgn"},
                                                     {"lsy", "sgn"},
                                                     {"ltg", "lv"},
                                                     {"lvs", "lv"},
                                                     {"lzh", "zh"},
                                                     {"max", "ms"},
                                                     {"mdl", "sgn"},
                                                     {"meo", "ms"},
                                                     {"mfa", "ms"},
                                                     {"mfb", "ms"},
                                                     {"mfs", "sgn"},
                                                     {"min", "ms"},
                                                     {"mnp", "zh"},
                                                     {"mqg", "ms"},
                                                     {"mre", "sgn"},
                                                     {"msd", "sgn"},
                                                     {"msi", "ms"},
                                                     {"msr", "sgn"},
                                                     {"mui", "ms"},
                                                     {"mzc", "sgn"},
                                                     {"mzg", "sgn"},
                                                     {"mzy", "sgn"},
                                                     {"nan", "zh"},
                                                     {"nbs", "sgn"},
                                                     {"ncs", "sgn"},
                                                     {"nsi", "sgn"},
                                                     {"nsl", "sgn"},
                                                     {"nsp", "sgn"},
                                                     {"nsr", "sgn"},
                                                     {"nzs", "sgn"},
                                                     {"okl", "sgn"},
                                                     {"orn", "ms"},
                                                     {"ors", "ms"},
                                                     {"pel", "ms"},
                                                     {"pga", "ar"},
                                                     {"pks", "sgn"},
                                                     {"prl", "sgn"},
                                                     {"prz", "sgn"},
                                                     {"psc", "sgn"},
                                                     {"psd", "sgn"},
                                                     {"pse", "ms"},
                                                     {"psg", "sgn"},
                                                     {"psl", "sgn"},
                                                     {"pso", "sgn"},
                                                     {"psp", "sgn"},
                                                     {"psr", "sgn"},
                                                     {"pys", "sgn"},
                                                     {"rms", "sgn"},
                                                     {"rsi", "sgn"},
                                                     {"rsl", "sgn"},
                                                     {"sdl", "sgn"},
                                                     {"sfb", "sgn"},
                                                     {"sfs", "sgn"},
                                                     {"sgg", "sgn"},
                                                     {"sgx", "sgn"},
                                                     {"shu", "ar"},
                                                     {"slf", "sgn"},
                                                     {"sls", "sgn"},
                                                     {"sqk", "sgn"},
                                                     {"sqs", "sgn"},
                                                     {"ssh", "ar"},
                                                     {"ssp", "sgn"},
                                                     {"ssr", "sgn"},
                                                     {"svk", "sgn"},
                                                     {"swc", "sw"},
                                                     {"swh", "sw"},
                                                     {"swl", "sgn"},
                                                     {"syy", "sgn"},
                                                     {"tmw", "ms"},
                                                     {"tse", "sgn"},
                                                     {"tsm", "sgn"},
                                                     {"tsq", "sgn"},
                                                     {"tss", "sgn"},
                                                     {"tsy", "sgn"},
                                                     {"tza", "sgn"},
                                                     {"ugn", "sgn"},
                                                     {"ugy", "sgn"},
                                                     {"ukl", "sgn"},
                                                     {"uks", "sgn"},
                                                     {"urk", "ms"},
                                                     {"uzn", "uz"},
                                                     {"uzs", "uz"},
                                                     {"vgt", "sgn"},
                                                     {"vkk", "ms"},
                                                     {"vkt", "ms"},
                                                     {"vsi", "sgn"},
                                                     {"vsl", "sgn"},
                                                     {"vsv", "sgn"},
                                                     {"wuu", "zh"},
                                                     {"xki", "sgn"},
                                                     {"xml", "sgn"},
                                                     {"xmm", "ms"},
                                                     {"xms", "sgn"},
                                                     {"yds", "sgn"},
                                                     {"ygs", "sgn"},
                                                     {"yhs", "sgn"},
                                                     {"ysl", "sgn"},
                                                     {"yue", "zh"},
                                                     {"zib", "sgn"},
                                                     {"zlm", "ms"},
                                                     {"zmi", "ms"},
                                                     {"zsl", "sgn"},
                                                     {"zsm", "ms"}};
const std::unordered_set<std::string> kW3CScript{
    "Adlm", "Afak", "Aghb", "Ahom", "Arab",       "Aran", "Armi", "Armn",
    "Avst", "Bali", "Bamu", "Bass", "Batk",       "Beng", "Blis", "Bopo",
    "Brah", "Brai", "Bugi", "Buhd", "Cakm",       "Cans", "Cari", "Cham",
    "Cher", "Cirt", "Copt", "Cprt", "Cyrl",       "Cyrs", "Deva", "Dsrt",
    "Dupl", "Egyd", "Egyh", "Egyp", "Elba",       "Ethi", "Geok", "Geor",
    "Glag", "Goth", "Gran", "Grek", "Gujr",       "Guru", "Hang", "Hani",
    "Hano", "Hans", "Hant", "Hatr", "Hebr",       "Hira", "Hluw", "Hmng",
    "Hrkt", "Hung", "Inds", "Ital", "Java",       "Jpan", "Jurc", "Kali",
    "Kana", "Khar", "Khmr", "Khoj", "Kitl",       "Kits", "Knda", "Kore",
    "Kpel", "Kthi", "Lana", "Laoo", "Latf",       "Latg", "Latn", "Lepc",
    "Limb", "Lina", "Linb", "Lisu", "Loma",       "Lyci", "Lydi", "Mahj",
    "Mand", "Mani", "Marc", "Maya", "Mend",       "Merc", "Mero", "Mlym",
    "Modi", "Mong", "Moon", "Mroo", "Mtei",       "Mult", "Mymr", "Narb",
    "Nbat", "Nkgb", "Nkoo", "Nshu", "Ogam",       "Olck", "Orkh", "Orya",
    "Osge", "Osma", "Palm", "Pauc", "Perm",       "Phag", "Phli", "Phlp",
    "Phlv", "Phnx", "Plrd", "Prti", "Qaaa..Qabx", "Rjng", "Roro", "Runr",
    "Samr", "Sara", "Sarb", "Saur", "Sgnw",       "Shaw", "Shrd", "Sidd",
    "Sind", "Sinh", "Sora", "Sund", "Sylo",       "Syrc", "Syre", "Syrj",
    "Syrn", "Tagb", "Takr", "Tale", "Talu",       "Taml", "Tang", "Tavt",
    "Telu", "Teng", "Tfng", "Tglg", "Thaa",       "Thai", "Tibt", "Tirh",
    "Ugar", "Vaii", "Visp", "Wara", "Wole",       "Xpeo", "Xsux", "Yiii",
    "Zinh", "Zmth", "Zsym", "Zxxx", "Zyyy",       "Zzzz"};
const std::unordered_set<std::string> kW3CRegion{
    "AA",     "AC",  "AD",  "AE",  "AF",  "AG",  "AI",  "AL",  "AM",  "AN",
    "AO",     "AQ",  "AR",  "AS",  "AT",  "AU",  "AW",  "AX",  "AZ",  "BA",
    "BB",     "BD",  "BE",  "BF",  "BG",  "BH",  "BI",  "BJ",  "BL",  "BM",
    "BN",     "BO",  "BQ",  "BR",  "BS",  "BT",  "BU",  "BV",  "BW",  "BY",
    "BZ",     "CA",  "CC",  "CD",  "CF",  "CG",  "CH",  "CI",  "CK",  "CL",
    "CM",     "CN",  "CO",  "CP",  "CR",  "CS",  "CU",  "CV",  "CW",  "CX",
    "CY",     "CZ",  "DD",  "DE",  "DG",  "DJ",  "DK",  "DM",  "DO",  "DZ",
    "EA",     "EC",  "EE",  "EG",  "EH",  "ER",  "ES",  "ET",  "EU",  "FI",
    "FJ",     "FK",  "FM",  "FO",  "FR",  "FX",  "GA",  "GB",  "GD",  "GE",
    "GF",     "GG",  "GH",  "GI",  "GL",  "GM",  "GN",  "GP",  "GQ",  "GR",
    "GS",     "GT",  "GU",  "GW",  "GY",  "HK",  "HM",  "HN",  "HR",  "HT",
    "HU",     "IC",  "ID",  "IE",  "IL",  "IM",  "IN",  "IO",  "IQ",  "IR",
    "IS",     "IT",  "JE",  "JM",  "JO",  "JP",  "KE",  "KG",  "KH",  "KI",
    "KM",     "KN",  "KP",  "KR",  "KW",  "KY",  "KZ",  "LA",  "LB",  "LC",
    "LI",     "LK",  "LR",  "LS",  "LT",  "LU",  "LV",  "LY",  "MA",  "MC",
    "MD",     "ME",  "MF",  "MG",  "MH",  "MK",  "ML",  "MM",  "MN",  "MO",
    "MP",     "MQ",  "MR",  "MS",  "MT",  "MU",  "MV",  "MW",  "MX",  "MY",
    "MZ",     "NA",  "NC",  "NE",  "NF",  "NG",  "NI",  "NL",  "NO",  "NP",
    "NR",     "NT",  "NU",  "NZ",  "OM",  "PA",  "PE",  "PF",  "PG",  "PH",
    "PK",     "PL",  "PM",  "PN",  "PR",  "PS",  "PT",  "PW",  "PY",  "QA",
    "QM..QZ", "RE",  "RO",  "RS",  "RU",  "RW",  "SA",  "SB",  "SC",  "SD",
    "SE",     "SG",  "SH",  "SI",  "SJ",  "SK",  "SL",  "SM",  "SN",  "SO",
    "SR",     "SS",  "ST",  "SU",  "SV",  "SX",  "SY",  "SZ",  "TA",  "TC",
    "TD",     "TF",  "TG",  "TH",  "TJ",  "TK",  "TL",  "TM",  "TN",  "TO",
    "TP",     "TR",  "TT",  "TV",  "TW",  "TZ",  "UA",  "UG",  "UM",  "US",
    "UY",     "UZ",  "VA",  "VC",  "VE",  "VG",  "VI",  "VN",  "VU",  "WF",
    "WS",
    "XA..XZ",  // TODO(w.kosowicz): should it be all the letters from XA to XZ?
    "YD",     "YE",  "YT",  "YU",  "ZA",  "ZM",  "ZR",  "ZW",  "ZZ",  "001",
    "002",    "003", "005", "009", "011", "013", "014", "015", "017", "018",
    "019",    "021", "029", "030", "034", "035", "039", "053", "054", "057",
    "061",    "142", "143", "145", "150", "151", "154", "155", "419"};
const std::unordered_set<std::string> kW3CVariant{
    "1606nict", "1694acad", "1901",     "1959acad", "1994",     "1996",
    "abl1943",  "alalc97",  "aluku",    "ao1990",   "arevela",  "arevmda",
    "baku1926", "balanka",  "barla",    "bauddha",  "biscayan", "biske",
    "bohoric",  "boont",    "colb1945", "dajnko",   "ekavsk",   "emodeng",
    "fonipa",   "fonupa",   "fonxsamp", "hepburn",  "heploc",   "hognorsk",
    "ijekavsk", "itihasa",  "jauer",    "jyutping", "kkcor",    "kociewie",
    "kscor",    "laukika",  "lipaw",    "luna1918", "metelko",  "monoton",
    "ndyuka",   "nedis",    "njiva",    "nulik",    "osojs",    "oxendict",
    "pamaka",   "petr1708", "pinyin",   "polyton",  "puter",    "rigik",
    "rozaj",    "rumgr",    "scotland", "scouse",   "solba",    "sotav",
    "surmiran", "sursilv",  "sutsilv",  "tarask",   "uccor",    "ucrcor",
    "ulster",   "unifon",   "vaidika",  "valencia", "vallader", "wadegile"};
}  // namespace w3c_languages
}  // namespace utils
