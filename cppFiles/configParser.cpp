#include "../headers/configParser.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void config(std::string config);

parser::parser(std::filesystem::path fileName) {
    if (fileName.extension() != ".forgecfg") {
        std::exit(1);
    }

    this->fileName = fileName;
    std::ifstream ifs(fileName);

    if (!ifs.is_open()) {
        std::ofstream ofs(fileName);
        ofs << parser::defaultConfig();
        ofs.close();

        ifs.open(fileName);
    }

    std::stringstream ss;
    ss << ifs.rdbuf();
    content = ss.str();
    ifs.close();

    config(this->content);
}

std::string parser::defaultConfig() {
    return R"(hash.value false
exeName.value app.exe
compileCommand.value g++)";
}

void parser::createFiles(std::filesystem::path file, std::string value) {
    std::filesystem::path wholePath = (std::filesystem::path) "." / ".FORGE" / ".DATA" / file;
    std::filesystem::create_directories(wholePath.parent_path());

    std::ofstream ofs(wholePath);
    ofs << value;
    ofs.close();
}

std::string parser::variableValueCreator(std::string valueName) {
    std::filesystem::path path = std::filesystem::path(".FORGE") / ".DATA" / (valueName + ".value");
    std::ifstream ifs(path);
    std::stringstream buffer;
    buffer << ifs.rdbuf();

    if (buffer.str() == "") {
        std::exit(1);
    }
    return buffer.str();
}

void parser::variableRewrite(std::string valueName, std::string value) {
    std::filesystem::path path = std::filesystem::path(".FORGE") / ".DATA" / (valueName + ".value");
    std::ofstream ofs(path);
    ofs << value;
    ofs.close();
}

parser p((std::filesystem::path) ".FORGE" / ".DATA" / "forge.forgecfg");

/*std::vector<parser>*/ void config(std::string config) {
    //std::vector<parser> cfg;
    bool wasSpace = false;
    std::string values[2];
    std::string tempVal = "";
    while (config.size() > 0) {
        //std::cout << tempVal << std::endl;
        if (config[0] != ' ' && config[0] != '\n') {
            tempVal += config[0];
        }
        else {
            int index = 0;
            if (wasSpace)
                index = 1;
            values[index] = tempVal;
            tempVal.clear();

            wasSpace = !wasSpace;

            if (index == 1) {
                p.createFiles(values[0], values[1]);
            }
        }

        config.erase(0, 1);
    }

    if (!tempVal.empty()) {
        int index = 0;
        if (wasSpace)
            index = 1;
        values[index] = tempVal;
        if (index == 1) {
            p.createFiles(values[0], values[1]);
        }
    }
}

void create() {
    config(p.content);
}

std::string cfgVals(std::string value) {
    return parser::variableValueCreator(value);
}