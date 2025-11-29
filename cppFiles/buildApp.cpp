#include "../headers/constants.h"
#include "../headers/funcs.h"
#include "../headers/configParser.h"
#include <fstream>
#include <iostream>
#include <string>

void desktopFileCreate(bool terminal, bool instaEnd){
    #ifdef __LINUX__
    std::string desktopFile = "";
    std::string name = cfgVals("exeName");
    std::filesystem::path exec = getExecFolder();
    std::string term = (terminal) ? "True" : "False";
    desktopFile += "[Desktop Entry]\n";
    desktopFile += "Type=Application\n";
    desktopFile += "Name= " + name + "\n";
    desktopFile += "Exec=\"" + std::filesystem::absolute((std::filesystem::path (".") / name).replace_extension("")).string() + "\"\n";
    desktopFile += "Terminal=" + term;

    std::cout << desktopFile;

    std::ofstream ofs(LINUXRESOURCES / std::filesystem::path(name).replace_extension(".desktop"));
    ofs << desktopFile;
    ofs.close();

    std::filesystem::path home = std::getenv("HOME");
    std::filesystem::create_directories(std::filesystem::path home / ".local" / "share" / "applications");
    std::ofstream ofs(std::filesystem::path home / ".local" / "share" / "applications" / std::filesystem::path(name).replace_extension(".desktop"));
    ofs << desktopFile;
    ofs.close();
    #endif
}