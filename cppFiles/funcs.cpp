#include <filesystem>
#include <fstream>  
#include <iostream>
#include "../headers/constants.h"
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <limits.h>
#include <unistd.h>
#include <pwd.h>
#endif

std::filesystem::path getExecFolder() {
#ifdef _WIN32
    char path[MAX_PATH];
    HMODULE hModule = GetModuleHandle(nullptr);
    if (hModule && GetModuleFileNameA(hModule, path, MAX_PATH)) {
        return std::filesystem::path(path).parent_path();
    }
#elifdef __linux__
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count != -1) {
        return std::filesystem::path(std::string(path, count));
    }
#elifdef __APPLE__
    char path[PATH_MAX];
    uint32_t size = sizeof(path);

    if (_NSGetExecutablePath(path, &size) == 0) {
        return std::filesystem::canonical(path).parent_path();
    }
#endif
    return {};
}

std::filesystem::path getConfigPath() {
#if defined(__linux__) || defined(__APPLE__)
    const char *homedir = getpwuid(getuid())->pw_dir;
    //std::cout << homedir << std::endl;
    return std::filesystem::path(std::string(homedir)) / ".config" / "FORGE";
#elif defined(_WIN32)
    std::cout << getExecFolder();
    return getExecFolder();
#endif
}

std::string getDefaultClangFile(){
    std::string defaultClang = R"(BasedOnStyle: LLVM
IndentWidth: 4

BasedOnStyle: LLVM
IndentWidth: 4
UseTab: Never
AccessModifierOffset: 0
AllowShortIfStatementsOnASingleLine: Never
AllowShortBlocksOnASingleLine: Never
AlwaysBreakAfterReturnType: None

BreakBeforeBraces: Custom
BraceWrapping:
  AfterControlStatement: false
  BeforeElse: true
  AfterFunction: false
  AfterClass: false
  AfterNamespace: true
  AfterStruct: true
  AfterEnum: true
  AfterUnion: true

ColumnLimit: 0 
MaxEmptyLinesToKeep: 1)";


    if (!std::filesystem::exists(std::filesystem::path(CONFIGFOLDER) / ".clang-format")){
        std::ofstream outfile (std::filesystem::path(CONFIGFOLDER) / ".clang-format");
        outfile << defaultClang << std::endl; outfile.close();
    }
    else if (!std::filesystem::exists(".clang-format")){
        std::ofstream outfile (".clang-format");
        outfile << defaultClang << std::endl; outfile.close();
    }
    else if (std::filesystem::exists(std::filesystem::path(CONFIGFOLDER) / ".clang-format")){
        std::ifstream t(CONFIGFOLDER / ".clang-format");
        std::stringstream buffer;
        buffer << t.rdbuf();
        defaultClang = buffer.str();
    }
    
    return defaultClang;
}