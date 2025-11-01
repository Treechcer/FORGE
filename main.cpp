#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::filesystem::path FORGEPATH = ".\\.FORGE";

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
            paths = getFiles(entry.path(), paths);
        }
    }

    return paths;
}

std::vector<std::filesystem::path> changePaths(std::vector<std::filesystem::path> paths) {
    std::cout << "----------------------------------------\n";
    for (int i = 0; i < paths.size(); i++){
        std::string temp = paths[i].string();
        temp.erase(0, 2);
        paths[i] = std::filesystem::path(FORGEPATH / temp);
        std::cout << temp << '\n';
        std::cout << paths[i] << '\n';
    }

    return paths;
}

int copyFiles(std::vector<std::filesystem::path> pathBefore, std::vector<std::filesystem::path> pathAfter) {
    if (pathAfter.size() != pathBefore.size()){
        return 1;
    }

    for (int i = 0; i < pathAfter.size(); i++){
        std::filesystem::create_directories(pathAfter[i].parent_path());
        std::filesystem::copy(pathBefore[i], pathAfter[i], std::filesystem::copy_options::overwrite_existing);
    }

    return 0;
}

int main() {
    std::vector<std::filesystem::path> paths;
    std::vector<std::filesystem::path> changedPaths;
    std::filesystem::path thisDir = ".";
    std::filesystem::create_directory(FORGEPATH);

    paths = getFiles(thisDir, paths);

    changedPaths = changePaths(paths);

    int status = copyFiles(paths, changedPaths);

    if (status == 1){
        return 0;
    }

    return 0;
}