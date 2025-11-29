#include "../headers/constants.h"
#include "../headers/funcs.h"
#include "../headers/configParser.h"
#include <fstream>
#include <iostream>
#include <string>

void desktopFileCreate(bool terminal, bool instaEnd){
    std::cout << "TEST OUTPUT\n";
#ifdef __linux__
    std::string desktopFile = "";
    std::string name = cfgVals("exeName");
    std::filesystem::path exec = getExecFolder();
    std::string term = (terminal) ? "true" : "false";
    std::string icon = (LINUXRESOURCES / "icon.png").string();

    desktopFile += "[Desktop Entry]\n";
    desktopFile += "Type=Application\n";
    desktopFile += "Name= " + name + "\n";
    desktopFile += "Exec=\"" + std::filesystem::absolute((std::filesystem::path (".") / name).replace_extension("")).string() + "\"\n";
    desktopFile += "Terminal=" + term;
    desktopFile += "Icon=" + icon;

    std::cout << desktopFile;

    std::ofstream ofs(LINUXRESOURCES / std::filesystem::path(name).replace_extension(".desktop"));
    ofs << desktopFile;
    ofs.close();

    std::filesystem::path home = std::getenv("HOME");
    std::filesystem::create_directories(home / ".local" / "share" / "applications");
    std::filesystem::path filePath = home / ".local" / "share" / "applications" / std::filesystem::path(name).replace_extension(".desktop");
    std::ofstream ofs1(filePath);
    ofs1 << desktopFile;
    ofs1.close();
    std::cout << home / ".local" / "share" / "applications";
#endif
}