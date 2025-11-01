#pragma once

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
};

void config(std::string config);
void create();
std::string cfgVals(std::string value);