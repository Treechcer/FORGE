#pragma once

#include <regex>
#include <filesystem>
#include <vector>
#include "../headers/funcs.h"
#include "../headers/configParser.h"

static std::string appName = cfgVals("exeName");
inline std::string noExeAppName = std::regex_replace(appName, std::regex("\\.exe$"), "");

//Folders for basic FORGE paths

const std::filesystem::path FORGEPATH = ".FORGE";
const std::filesystem::path FORGEPROJECTPATH = FORGEPATH / ".PROJECT";
const std::filesystem::path FORGEDATAPATH = FORGEPATH / ".DATA";

//Folders for ./LIBS

const std::filesystem::path LIBS = "LIBS";
const std::filesystem::path STATICLIBS = LIBS / "STATIC";
const std::filesystem::path DYNAMICLIBS = LIBS / "DYNAMIC";
const std::filesystem::path LIBCOMPILE = LIBS / "COMPILED";
const std::filesystem::path LIBSOURCE = LIBS / "SOURCE";

//Folders for ./.FORGE/LIBS...

const std::filesystem::path LIBDOTFORGE = FORGEPROJECTPATH / "LIBS";
const std::filesystem::path LIBFORGECOPIED = LIBDOTFORGE / "STATIC";
const std::filesystem::path LIBDOTFORGESRC = LIBDOTFORGE / "SOURCE";

//there are just GLOBALS that won't change values after value asighnment, they might statr with some value and then they cahnge

inline std::string COMPILERCOMMAND; // I have to assighn this later, that's why it's not const, but it'll be treated like one
inline std::filesystem::path CONFIGFOLDER = getConfigPath();
inline std::string LIBRARIES = "";

//cpp file suffixes / extensions

inline std::vector <std::regex> CPPPOSSIBLESOURCEFILES = {std::regex(".*\\.cpp$"), std::regex(".*\\.cc$"), std::regex(".*\\.cxx$"), std::regex(".*\\.c++$"), std::regex(".*\\.cp$")};
inline std::vector <std::regex> CPPPOSSIBLEHEADERFILES = {std::regex(".*\\.h$"), std::regex(".*\\.hpp$"), std::regex(".*\\.hh$"), std::regex(".*\\.hxx$"), std::regex(".*\\.h++$")};

//C file suffixes / extensions

inline std::vector<std::regex> CPOSSIBLESOURCEFILES = {std::regex(".*\\.c$")};
inline std::vector<std::regex> CPOSSIBLEHEADERFILES = {std::regex(".*\\.h$")};

//linux RSC

inline std::filesystem::path LINUXRESOURCES = (std::filesystem::path) "." / "linuxResources";
inline std::filesystem::path LINUXAPPRESOURCES = LINUXRESOURCES / "APPS";
//mac RSC

inline std::filesystem::path MACRESOURCES = (std::filesystem::path) "." / "macResources";
inline std::filesystem::path MACAPPRESOURCES = MACRESOURCES / "APPS";

inline std::filesystem::path MACCONTENTS = MACAPPRESOURCES / (noExeAppName + ".app") / "Contents";
inline std::filesystem::path MACCONTENTSMACOS = MACCONTENTS / "MacOS";
inline std::filesystem::path MACCONTENTSRESOURCES = MACCONTENTS / "Resources";