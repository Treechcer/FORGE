#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

size_t hashString(std::string toHash) {
    std::string text = toHash;
    std::hash<std::string> hasher;
    size_t hashValue = hasher(text);
    std::cout << hashValue << std::endl;
    return hashValue;
}

std::vector <std::string> getFilesToCopy(std::string path = "."){
    std::string path = path;
    std::vector<std::string> toCopy;
    for (const auto &entry : std::filesystem::directory_iterator(path)) { // thanks stack overflow!!
        //std::cout << entry.path() << std::endl;

        std::string file = entry.path().string();

        //std::cout << file << std::endl;

        if (std::regex_search(file, std::regex(".cpp$"))) {
            toCopy.push_back(file);
        }
        else if (std::regex_search(file, std::regex(".h$"))) {
            toCopy.push_back(file);
        }
    }
}

int main() {
    std::vector<std::string> toCopy = getFilesToCopy();

    for (int i = 0; i < toCopy.size(); i++) { // debug / temp writing out files
        std::cout << toCopy[i] << std::endl;
    }

    std::filesystem::create_directories("./.temp"); // for creating the build folder (called .temp for now)
    
	for (int i = 0; i < toCopy.size(); i++) {
        std::ifstream src(toCopy[i], std::ios::binary);
        std::ofstream dst(std::filesystem::path(".temp") / toCopy[i], std::ios::binary);
        dst << src.rdbuf();
    }

    return 0;
}