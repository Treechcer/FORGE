#include "../headers/configParser.h"
#include "../headers/constants.h"
#include "../headers/funcs.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

void config(std::string config);

parser::parser(std::filesystem::path fileName) {
    if (fileName.extension() != ".forgecfg") {
        std::exit(1);
    }

    std::filesystem::path execFolder = getExecFolder();

    if (!std::filesystem::exists(CONFIGFOLDER / "forge.forgecfg")) {
        std::ofstream ofs(CONFIGFOLDER / "forge.forgecfg");
        ofs << parser::defaultConfig();
    }

    if (!std::filesystem::exists(FORGEDATAPATH / "forge.forgecfg")) {
        std::filesystem::create_directories(FORGEDATAPATH);
        std::filesystem::copy(CONFIGFOLDER / "forge.forgecfg", FORGEDATAPATH / "forge.forgecfg", std::filesystem::copy_options::overwrite_existing);
        create();
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
#ifdef __APPLE__
    return R"(hash false
exeName "forge app.exe" -KEEP
compileCommand "clang++ --std=c++17"
createClangFile true
threads 4
language C++
compileCommandC clang)";
#else
    return R"(hash false
exeName "forge app.exe" -KEEP
compileCommand g++
createClangFile true
threads 4
language C++
compileCommandC gcc)";
#endif
}

void parser::createFiles(std::filesystem::path file, std::string value) {
#if defined(__linux__) || defined(__APPLE__)
    // this should work... I think lol - it didn't, last famous words
    if (file == "exeName") {
        value = std::regex_replace(value, std::regex("\\.exe$"), "");
        value = std::regex_replace(value, std::regex("\\.exe'$"), "'");
        value = std::regex_replace(value, std::regex("\\.exe\"$"), "\"");
    }
#endif
    std::filesystem::path wholePath = FORGEDATAPATH / file;
    std::filesystem::create_directories(wholePath.parent_path());

    std::ofstream ofs(wholePath);
    ofs << value;
    ofs.close();
}

std::string parser::variableValueCreator(std::string valueName) {
    std::filesystem::path path = FORGEDATAPATH / (valueName);
    std::ifstream ifs(path);
    std::stringstream buffer;
    buffer << ifs.rdbuf();

    if (buffer.str() == "") {
        std::exit(1);
    }
    return buffer.str();
}

void parser::variableRewrite(std::string valueName, std::string value) {
    std::filesystem::path path = FORGEDATAPATH / (valueName);
    std::ofstream ofs(path);
    ofs << value;
    ofs.close();
}

parser p(FORGEDATAPATH / "forge.forgecfg");

/*std::vector<parser>*/ void config(std::string config) {
    //std::vector<parser> cfg;
    //bool wasSpace = false;
    //std::string values[2];
    //std::string tempVal = "";

    std::stringstream ss(config);
    std::string line;
    while (std::getline(ss, line)) {
        //getting the 'key'
        int indexer = 0;
        std::string key = "";
        while (line[indexer] != ' ') {
            key += line[indexer];
            indexer++;
        }

        //key = std::regex_replace(key, std::regex("\\.value"), "");

        //std::cout << key + " : ";
        indexer++;

        std::string value = "";

        bool quote = line[indexer] == '"' || line[indexer] == '\'';
        char quoteType = ' ';
        if (quote) {
            quoteType = line[indexer];
            indexer++;
            value += quoteType;
        }

        while (indexer < line.size() && ((quote && line[indexer] != '"') || (!quote && line[indexer] != ' '))) {
            value += line[indexer];
            indexer++;
        }

        if (quote) {
            value += quoteType;
            indexer++;
        }

        std::string flag = "";
        indexer++;
        if (indexer < line.size()) {
            while (indexer < line.size() && line[indexer] != ' ') {
                flag += line[indexer];
                indexer++;
            }
        }

        if (!(flag == "-keep" || flag == "-KEEP")) {
            if (value[0] == '"' || value[0] == '\'') {
                value.erase(0, 1);
            }
            if (value[value.size() - 1] == '"' || value[value.size() - 1] == '\'') {
                value.erase(value.size() - 1, 1);
            }
        }
        //std::cout << value << std::endl;
        //std::cout << flag << std::endl << quoteType << std::endl << quote << std::endl;

        p.createFiles(key, value);
    }

    //while (config.size() > 0) {
    //    //std::cout << tempVal << std::endl;
    //    if (config[0] != ' ' && config[0] != '\n' && config[0] != '\'') {
    //        tempVal += config[0];
    //    }
    //    else if (config[0] == '\'') {
    //        config.erase(0, 1);
    //        while (config[0] != '\'') {
    //            tempVal += config[0];
    //            config.erase(0, 1);
    //        }
    //        if (config[0] == ' '){
    //            config.erase(0, 1);
    //        }
    //        std::string flag = "";
    //        while (config[0] != '\n' && config[0] != ' ') {
    //            if (config[0] != ' ') {
    //                flag = config[0];
    //            }
    //            config.erase(0, 1);
    //        }
    //        if (flag == "-KEEP"){
    //            tempVal += '\'';
    //            tempVal.insert(0, "'");
    //        }
    //    }
    //    else {
    //        int index = 0;
    //        if (wasSpace)
    //            index = 1;
    //        values[index] = tempVal;
    //        tempVal.clear();
    //
    //        wasSpace = !wasSpace;
    //
    //        if (index == 1) {
    //            p.createFiles(values[0], values[1]);
    //        }
    //    }
    //
    //    config.erase(0, 1);
    //}
    //
    //if (!tempVal.empty()) {
    //    int index = 0;
    //    if (wasSpace)
    //        index = 1;
    //    values[index] = tempVal;
    //    if (index == 1) {
    //        p.createFiles(values[0], values[1]);
    //    }
    //}
}

void create() {
    config(p.content);
}

std::string cfgVals(std::string value) {
    return parser::variableValueCreator(value);
}