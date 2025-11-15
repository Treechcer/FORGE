#pragma once

#include "../headers/funcs.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class parser {
    public:
    std::filesystem::path fileName;
    std::string content;
    parser(std::filesystem::path fileName);
    void static createFiles(std::filesystem::path file, std::string value);
    std::string static variableValueCreator(std::string valueName);
    static std::string defaultConfig();
    static void variableRewrite(std::string valueName, std::string value);
};

void config(std::string config);
void create();
std::string cfgVals(std::string value);