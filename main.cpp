#include <cstdlib>
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
    std::vector<std::string> toCopy;
    for (const auto &entry : std::filesystem::directory_iterator(path)) { // thanks stack overflow!!
        //std::cout << entry.path() << std::endl;

        std::string file = entry.path().string();

        //std::cout << file << std::endl;

        if (std::regex_search(file, std::regex("\\.cpp$"))) {
            toCopy.push_back(file);
        }
        else if (std::regex_search(file, std::regex("\\.h$"))) {
            toCopy.push_back(file);
        }
        else if (entry.is_directory() && std::regex_search(file, std::regex(".\\.path"))) {
            std::vector<std::string> subFiles = getFilesToCopy(file);
            toCopy.insert(toCopy.end(), subFiles.begin(), subFiles.end());
        }
    }

    return toCopy;
}

int main() {
    std::vector<std::string> toCopy = getFilesToCopy();

    for (int i = 0; i < toCopy.size(); i++) { // debug / temp writing out files
        std::cout << toCopy[i] << std::endl;
    }

    std::filesystem::create_directories("./.temp"); // for creating the build folder (called .temp for now)
    
	for (int i = 0; i < toCopy.size(); i++) {
        std::filesystem::create_directories(std::filesystem::path(".temp") / std::filesystem::path(toCopy[i]).parent_path());

        std::ifstream ifs(toCopy[i]);
        std::string contentPNew((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        //std::cout << hashString(contentPNew) << std::endl;

        size_t hashPNew = hashString(contentPNew);

        std::ifstream ifs2(std::filesystem::path(".temp") / toCopy[i].substr(2));
        std::string contentPold((std::istreambuf_iterator<char>(ifs2)), (std::istreambuf_iterator<char>()));
        //std::cout << hashString(contentPold) << std::endl;

        size_t hashPOld = hashString(contentPold);

        if (hashPNew != hashPOld){
            std::ifstream src(toCopy[i], std::ios::binary);
            std::filesystem::path path = std::filesystem::path(".temp") / toCopy[i].substr(2);
            std::ofstream dst(path, std::ios::binary);
            dst << src.rdbuf();

            std::string cmd = "g++ -c ";
            cmd.append(path.string());
            cmd.append(" -o ");
            cmd.append(std::filesystem::path(".temp/").string());
            cmd.append(toCopy[i].substr(2, toCopy[i].size() - 5));
            cmd.append("o");

            std::cout << cmd << std::endl;

            system(cmd.c_str());
        }
    }

    return 0;
}