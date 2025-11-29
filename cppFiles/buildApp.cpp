#include "../headers/constants.h"
#include "../headers/funcs.h"
#include "../headers/configParser.h"
#include <fstream>
#include <iostream>
#include <string>

void desktopFileCreate(bool terminal, bool instaEnd){
    std::cout << "TEST OUTPUT\n";
#ifdef __LINUX__
    std::string desktopFile = "";
    std::string name = cfgVals("exeName");
    std::filesystem::path exec = getExecFolder();
    std::string term = (terminal) ? "true" : "false";
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
    std::filesystem::create_directories(home / ".local" / "share" / "applications");
    std::ofstream ofs1(std::filesystem::path home / ".local" / "share" / "applications" / std::filesystem::path(name).replace_extension(".desktop"));
    ofs1 << desktopFile;
    ofs1.close();
    std::cout << home / ".local" / "share" / "applications";
#endif
}