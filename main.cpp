#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

size_t hashString(std::string toHash) {
    std::string text = toHash;
    std::hash<std::string> hasher;
    size_t hashValue = hasher(text);
    //std::cout << hashValue << std::endl;
    return hashValue;
}

std::vector<std::filesystem::path> getFiles(std::filesystem::path dir, std::vector<std::filesystem::path> paths) {
    if (std::regex_match(dir.string(), std::regex(".*\\.git")) || std::regex_match(dir.string(), std::regex(".*\\.FORGE"))) {
        return paths;
    }
    for (const auto &entry : std::filesystem::directory_iterator(dir)) {
        std::string str = entry.path().string();
        //std::cout << str << std::endl;
        if (std::regex_match(str, std::regex(".*\\.cpp$")) || std::regex_match(str, std::regex(".*\\.h$"))) {
            std::cout << entry.path() << "\n";
            paths.push_back(entry.path());
        }
        else if (entry.is_directory()){
            getFiles(entry.path(), paths);
        }
    }

    return paths;
}

int main() {
    std::vector<std::filesystem::path> paths;
    std::filesystem::path thisDir = ".";
    std::filesystem::create_directory(".\\.FORGE");

    getFiles(thisDir, paths);

    return 0;
}