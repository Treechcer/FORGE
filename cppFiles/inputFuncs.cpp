#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

void update(std::string compileCmd, std::filesystem::path execPath) {
    if (std::filesystem::exists((std::filesystem::path) "." / ".FORGE" / ".UPDATE")) {
        system("git pull https://github.com/Treechcer/FORGE");
    }
    else{
        std::filesystem::create_directory((std::filesystem::path) "." / ".FORGE" / ".UPDATE");
        std::filesystem::path target = (std::filesystem::path) "." / ".FORGE" / ".UPDATE";
        system(("git clone https://github.com/Treechcer/FORGE " + target.string()).c_str());
    }

    std::filesystem::current_path(std::filesystem::current_path() / ".FORGE" / ".UPDATE");

    std::string cmd = execPath.string() + " -path " + execPath.parent_path().string();
    system(cmd.c_str());

    //std::filesystem::remove_all(std::filesystem::current_path());
    /*
    this scrashes the programme, I will be honest, I don't feel like making optimised
    way of deleting it to save a few MBs, you can just delete it yourself ngl
    */
}