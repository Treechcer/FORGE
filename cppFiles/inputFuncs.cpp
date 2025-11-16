#include "../headers/constants.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

void update(std::string compileCmd, std::filesystem::path execPath) {
    if (std::filesystem::exists((std::filesystem::path) "." / ".FORGE" / ".UPDATE")) {
        system("git pull https://github.com/Treechcer/FORGE");
    }
    else {
        std::filesystem::create_directory((std::filesystem::path) "." / ".FORGE" / ".UPDATE");
        std::filesystem::path target = (std::filesystem::path) "." / ".FORGE" / ".UPDATE";
        system(("git clone https://github.com/Treechcer/FORGE " + target.string()).c_str());
    }

    //std::filesystem::current_path(std::filesystem::current_path() / ".FORGE" / ".UPDATE");

    //std::string cmd = execPath.string() + " -path " + execPath.parent_path().string();
    //system(cmd.c_str());

    std::string cmd = "";
#ifdef __WIN32__
    std::string end = ".exe";
#else
    std::string end = "";
#endif

    cmd += COMPILERCOMMAND + " " + std::filesystem::path(FORGEPATH / ".UPDATE" / "cppFiles" / "*.cpp").string() + " -o forge" + end;

    std::cout << cmd << std::endl;

    system(cmd.c_str());

    std::exit(0);
}