#pragma once

#include <filesystem>
#include "../headers/funcs.h"

const std::filesystem::path FORGEPATH = ".FORGE";
const std::filesystem::path FORGEPROJECTPATH = FORGEPATH / ".PROJECT";
const std::filesystem::path FORGEDATAPATH = FORGEPATH / ".DATA";

const std::filesystem::path LIBS = "LIBS";
const std::filesystem::path STATICLIBS = LIBS / "STATIC";
const std::filesystem::path DYNAMICLIBS = LIBS / "DYNAMIC";
const std::filesystem::path LIBCOMPILE = LIBS / "COMPILED";
const std::filesystem::path LIBSOURCE = LIBS / "SOURCE";

const std::filesystem::path LIBFORGECOPIED = FORGEPATH / "LIBS" / "STATIC";

//there are just GLOBALS that won't change values after value asighnment, they might statr with some value and then they cahnge

inline std::string COMPILERCOMMAND; // I have to assighn this later, that's why it's not const, but it'll be treated like one
inline std::filesystem::path CONFIGFOLDER = getConfigPath();
inline std::string LIBRARIES = "";