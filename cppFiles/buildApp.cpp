#include "../headers/constants.h"
#include "../headers/funcs.h"
#include "../headers/inputFuncs.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

void desktopFileCreate(bool terminal, bool instaEnd){
    std::cout << "TEST OUTPUT\n";

#ifdef __linux__
    std::string desktopFile = "";
    std::string name = cfgVals("exeName");
    std::filesystem::path exec = getExecFolder();
    std::string term = (terminal) ? "true" : "false";
    std::string icon = (std::filesystem::absolute(LINUXRESOURCES) / "icon.png").lexically_normal().string();
    std::string preExec = std::regex_replace(std::filesystem::absolute((std::filesystem::path(name)).replace_extension("")).string(), std::regex(" "), "\\ ");
    std::string exec = (instaEnd) ? preExec : "bash -c \"" + preExec + "; read -p 'Press Enter to exit...'\"";

    desktopFile += "[Desktop Entry]\n";
    desktopFile += "Type=Application\n";
    desktopFile += "Name= " + name + "\n";
    desktopFile += "Exec=" + exec + "\n";
    desktopFile += "Terminal=" + term + "\n";
    desktopFile += "Icon=" + icon + "\n";

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
#elif defined (__APPLE__)
    std::cout << "comming soon..."
#endif
}