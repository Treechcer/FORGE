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
    //std::cout << hashValue << std::endl;
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
        else if (entry.is_directory() && !(std::regex_search(file, std::regex(".\\.temp")))) {
            std::vector<std::string> subFiles = getFilesToCopy(file);
            toCopy.insert(toCopy.end(), subFiles.begin(), subFiles.end());
        }
    }

    return toCopy;
}

void buildProject(std::vector <std::string> objFile, std::string name="FORGE.exe"){
    std::string cmdObjpart = "";
    for (int i = 0; i < objFile.size(); i++){
        cmdObjpart.append(objFile[i]);
        cmdObjpart.append(" ");
    }
    
    //std::cout << cmdObjpart;

    std::string cmd = "g++ ";
    cmd.append(cmdObjpart);
    cmd.append("-o ");
    cmd.append(name);
    //cmd.append(" -mconsole");

    for (int i = 0; i < cmd.size(); i++){
        if (cmd[i] == '\\'){
            cmd[i] = '/';
        }
    }
    
    std::cout << cmd << std::endl;
    system(cmd.c_str());
}

int main() {
    std::vector<std::string> toCopy = getFilesToCopy();

    //for (int i = 0; i < toCopy.size(); i++) { // debug / temp writing out files
    //    std::cout << toCopy[i] << std::endl;
    //}

    std::filesystem::create_directories("./.temp"); // for creating the build folder (called .temp for now)

    size_t hashEmpty = hashString("");

    std::vector<std::string> objFiles;

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
        std::string file = std::filesystem::path(".temp\\").string() + toCopy[i].substr(2, toCopy[i].size() - 5) + "o";
        objFiles.push_back(file); //

        if (hashPNew != hashPOld && hashPNew != hashEmpty){
            std::ifstream src(toCopy[i], std::ios::binary);
            std::filesystem::path path = std::filesystem::path(".temp") / toCopy[i].substr(2);
            std::ofstream dst(path, std::ios::binary);
            dst << src.rdbuf();
            
            std::string cmd = "g++ -c ";
            cmd.append(path.string());
            cmd.append(" -o ");
            cmd.append(std::filesystem::path (file).string());

            //cmd.append(std::filesystem::path(".temp\\").string());
            //cmd.append(toCopy[i].substr(2, toCopy[i].size() - 5));
            //cmd.append("o");

            std::cout << cmd << std::endl;

            //std::cout << file << std::endl;

            //for (int i = 0; i < cmd.size(); i++) {
            //    if (cmd[i] == '\\') {
            //        cmd[i] = '/';
            //    }
            //}

            system(cmd.c_str());
        }
    }

    buildProject(objFiles);

    return 0;
}