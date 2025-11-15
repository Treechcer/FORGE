#pragma once

#include <filesystem>

const std::filesystem::path FORGEPATH = ".FORGE";
const std::filesystem::path FORGEPROJECTPATH = FORGEPATH / ".PROJECT";
const std::filesystem::path FORGEDATAPATH = FORGEPATH / ".DATA";

extern std::string COMPILERCOMMAND; // I have to assighn this later, that's why it's not const, but it'll be treated like one