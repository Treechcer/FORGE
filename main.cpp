#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

void compileOne(std::filesystem::path pathAfter);
void buildPorject(std::vector<std::filesystem::path> pathAfter);

std::filesystem::path FORGEPATH = ".\\.FORGE";
std::filesystem::path FORGEPROJECTPATH = ".\\.FORGE\\\\.PROJECT";
std::filesystem::path FORGEDATAPATH = ".\\.FORGE\\\\.DATA";

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
    //std::cout << "----------------------------------------\n";
    for (int i = 0; i < paths.size(); i++){
        std::string temp = paths[i].string();
        temp.erase(0, 2);
        paths[i] = std::filesystem::path(FORGEPROJECTPATH / temp);
        //std::cout << temp << '\n';
        //std::cout << paths[i] << '\n';
    }

    return paths;
}

int copyFiles(std::vector<std::filesystem::path> pathBefore, std::vector<std::filesystem::path> pathAfter) {
    if (pathAfter.size() != pathBefore.size()){
        return 1;
    }

    //size_t empyStr = hashString(""); // -- might use later

    for (int i = 0; i < pathAfter.size(); i++){
        std::ifstream f1(pathBefore[i]);
        std::stringstream buffer;
        buffer << f1.rdbuf();
        size_t beforeHash = hashString(buffer.str());
        f1.close();

        buffer.str("");
        buffer.clear();

        std::ifstream f2(pathAfter[i]);
        buffer << f2.rdbuf();
        size_t afterHash = hashString(buffer.str());
        f2.close();

        if ((afterHash != beforeHash)) {
            std::filesystem::create_directories(pathAfter[i].parent_path());
            std::filesystem::copy(pathBefore[i], pathAfter[i], std::filesystem::copy_options::overwrite_existing);

            compileOne(pathAfter[i]);
        }
    }

    return 0;
}

void compileAll(std::vector<std::filesystem::path> pathAfter) {
    //std::cout << changedPaths[0].filename() << " " << changedPaths[0].parent_path() / changedPaths[0].replace_extension(".o");

    for (int i = 0; i < pathAfter.size(); i++){
        //g++ changedPaths[0] -o changedPaths[0].parent_path() / changedPaths[0].replace_extension(".o")
        //if (pathAfter[i].extension()){}

        if (pathAfter[i].extension() == ".h") {
            break;
        }

        std::string cmd = "g++ -c ";
        cmd.append(pathAfter[i].string());
        cmd.append(" -o ");
        cmd.append(pathAfter[i].replace_extension(".o").string());
        //std::cout << cmd << std::endl;
        system(cmd.c_str());
    }
}

void compileOne(std::filesystem::path pathAfter) {
    if (pathAfter.extension() == ".h"){
        return;
    }

    std::string cmd = "g++ -c ";
    cmd.append(pathAfter.string());
    cmd.append(" -o ");
    cmd.append(pathAfter.replace_extension(".o").string());
    //std::cout << cmd << std::endl;
    system(cmd.c_str());
}

void buildPorject(std::vector<std::filesystem::path> pathAfter) {
    std::string allObJs = "";
    for (int i = 0; i < pathAfter.size(); i++){
        if (pathAfter[i].extension() == ".h") {
            break;
        }
        allObJs.append(pathAfter[i].replace_extension(".o").string() + " ");
    }

    //std::cout << allObJs << "\n";

    std::string cmd = "g++ " + allObJs + "-o app.exe";
    system(cmd.c_str());
    //std::cout << cmd;
}

int main() {
    std::vector<std::filesystem::path> paths;
    std::vector<std::filesystem::path> changedPaths;
    std::filesystem::path thisDir = ".";
    std::filesystem::create_directory(FORGEPATH);
    std::filesystem::create_directory(FORGEPROJECTPATH);
    std::filesystem::create_directory(FORGEDATAPATH);

    paths = getFiles(thisDir, paths);

    changedPaths = changePaths(paths);

    int status = copyFiles(paths, changedPaths);

    if (status == 1){
        return 0;
    }

    //compileAll(changedPaths);

    buildPorject(changedPaths);

    //for (int i = 0; i < changedPaths.size(); i++){
    //    std::cout << changedPaths[i].extension();
    //}

    return 0;
}