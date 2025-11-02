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
    parser(std::filesystem::path fileName) {
        if (fileName.extension() != ".forgecfg") {
            std::cout << "incorrect file extension";
            std::exit(1);
        }

        this->fileName = fileName;
        std::ifstream ifs(fileName);

        if (!ifs.is_open()){
            std::ofstream ofs(fileName);
            ofs << parser::defaultConfig();
        }

        std::stringstream ss;
        ss << ifs.rdbuf();
        content = ss.str();
        ifs.close();

        this->content = content;
    }

    static std::string defaultConfig(){
        return R"(hash.value false
exeName.value app.exe)";
    }

    static void createFiles(std::filesystem::path file, std::string value) {
        std::filesystem::path wholePath = ".\\.FORGE\\\\.DATA" / file;
        std::filesystem::create_directories(wholePath.parent_path());

        std::ofstream ofs(wholePath);
        ofs << value;
        ofs.close();
    }

    static std::string variableValueCreator(std::string valueName) {
        std::filesystem::path path = std::filesystem::path(".FORGE") / ".DATA" / (valueName + ".value");
        std::ifstream ifs(path);
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::cout << path;
        if (buffer.str() == ""){
            std::cout << "file is empty or doesn't exist";
            std::exit(1);
        }
        return buffer.str();
    }
};

parser p(std::string("forge.forgecfg"));

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

std::string cfgVals(std::string value){
    return parser::variableValueCreator(value);
}