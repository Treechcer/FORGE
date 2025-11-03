#include "configParser.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <limits.h>
    #include <unistd.h>
#endif

void compileOne(std::filesystem::path pathAfter);
void buildPorject(std::vector<std::filesystem::path> pathAfter);

std::filesystem::path FORGEPATH = ".FORGE";
std::filesystem::path FORGEPROJECTPATH = FORGEPATH / "PROJECT";
std::filesystem::path FORGEDATAPATH = FORGEPATH / ".DATA";
bool HASH = false;

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
        else if (entry.is_directory()) {
            paths = getFiles(entry.path(), paths);
        }
    }

    return paths;
}

std::vector<std::filesystem::path> changePaths(std::vector<std::filesystem::path> paths) {
    //std::cout << "----------------------------------------\n";
    for (int i = 0; i < paths.size(); i++) {
        std::string temp = paths[i].string();
        temp.erase(0, 2);
        paths[i] = std::filesystem::path(FORGEPROJECTPATH / temp);
        //std::cout << temp << '\n';
        //std::cout << paths[i] << '\n';
    }

    return paths;
}

int copyFiles(std::vector<std::filesystem::path> pathBefore, std::vector<std::filesystem::path> pathAfter) {
    if (pathAfter.size() != pathBefore.size()) {
        return 1;
    }

    //size_t empyStr = hashString(""); // -- might use later

    for (int i = 0; i < pathAfter.size(); i++) {
        bool copy = false;

        if (HASH) {
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

            if (afterHash != beforeHash) {
                copy = true;
            }
        }
        else {
            if (!(std::filesystem::exists(pathAfter[i]))) {
                copy = true;
            }
            else if (std::filesystem::last_write_time(pathAfter[i]) != std::filesystem::last_write_time(pathBefore[i])) {
                copy = true;
            }
        }

        if (copy) {
            std::filesystem::create_directories(pathAfter[i].parent_path());
            std::filesystem::copy(pathBefore[i], pathAfter[i], std::filesystem::copy_options::overwrite_existing);

            compileOne(pathAfter[i]);
        }
    }

    return 0;
}

void compileAll(std::vector<std::filesystem::path> pathAfter) {
    //std::cout << changedPaths[0].filename() << " " << changedPaths[0].parent_path() / changedPaths[0].replace_extension(".o");

    for (int i = 0; i < pathAfter.size(); i++) {
        //g++ changedPaths[0] -o changedPaths[0].parent_path() / changedPaths[0].replace_extension(".o")
        //if (pathAfter[i].extension()){}

        if (pathAfter[i].extension() == ".h") {
            continue;
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
    if (pathAfter.extension() == ".h") {
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
    for (int i = 0; i < pathAfter.size(); i++) {
        if (pathAfter[i].extension() == ".h") {
            continue;
            ;
        }
        allObJs.append(pathAfter[i].replace_extension(".o").string() + " ");
    }

    //std::cout << allObJs << "\n";
    std::string appName = cfgVals("exeName");
    std::string cmd = "g++ " + allObJs + "-o " + appName;
    std::cout << cmd;
    system(cmd.c_str());
    //std::cout << cmd;
}

bool strToBool(std::string strBool) {
    if (strBool == "true")
        return true;
    return false;
}

std::filesystem::path getExecFolder() {
#if defined(_WIN32)
    char path[MAX_PATH];
    HMODULE hModule = GetModuleHandle(nullptr);
    if (hModule && GetModuleFileNameA(hModule, path, MAX_PATH)) {
        return std::filesystem::path(path);
    }
#elif defined(__linux__)
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count != -1) {
        return std::filesystem::path(std::string(path, count));
    }
#endif
    return {};
}

int main() {
    std::filesystem::path execFolder =  getExecFolder();
    if (std::filesystem::exists(execFolder.parent_path() / "forge.forgecfg")){
        std::ofstream ofs(execFolder.parent_path() / "forge.forgecfg");
        ofs << parser::defaultConfig();
    }
    std::filesystem::copy(execFolder.parent_path() / "forge.forgecfg", FORGEDATAPATH / "forge.forgecfg", std::filesystem::copy_options::overwrite_existing);
    return 0;
    std::vector<std::filesystem::path> paths;
    std::vector<std::filesystem::path> changedPaths;
    std::filesystem::path thisDir = ".";
    std::filesystem::create_directory(FORGEPATH);
#ifdef _WIN32
    SetFileAttributesA(FORGEPATH.string().c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
#endif

    std::filesystem::create_directory(FORGEPROJECTPATH);
    std::filesystem::create_directory(FORGEDATAPATH);

    create();

    HASH = strToBool(cfgVals("hash"));

    paths = getFiles(thisDir, paths);

    changedPaths = changePaths(paths);

    int status = copyFiles(paths, changedPaths);

    if (status == 1) {
        return 0;
    }

    //compileAll(changedPaths);

    buildPorject(changedPaths);

    //for (int i = 0; i < changedPaths.size(); i++){
    //    std::cout << changedPaths[i].extension();
    //}

    return 0;
}