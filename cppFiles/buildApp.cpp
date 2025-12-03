#include "../headers/constants.h"
#include "../headers/funcs.h"
#include "../headers/configParser.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

void desktopFileCreate(bool terminal, bool instaEnd, std::string version = "not set - FORGE default"){
    std::cout << "TEST OUTPUT\n";
#ifdef __linux__
    std::filesystem::create_directories(LINUXAPPRESOURCES);
    std::string desktopFile = "";
    std::string name = cfgVals("exeName");
    std::filesystem::path exec = getExecFolder();
    std::string term = (terminal) ? "true" : "false";
    std::string icon = (std::filesystem::absolute(LINUXRESOURCES) / "FORGE.png").lexically_normal().string();
    std::string preExec = std::regex_replace(std::filesystem::absolute((std::filesystem::path(name)).replace_extension("")).string(), std::regex(" "), "\\ ");
    std::string execC = (instaEnd) ? preExec : "bash -c \"" + preExec + "\"; read -p 'Press Enter to exit...'\"";

    desktopFile += "[Desktop Entry]\n";
    desktopFile += "Type=Application\n";
    desktopFile += "Name= " + name + "\n";
    desktopFile += "Exec=" + execC + "\n";
    desktopFile += "Terminal=" + term + "\n";
    desktopFile += "Icon=" + icon + "\n";

    std::cout << desktopFile;

    std::ofstream ofs(LINUXAPPRESOURCES / std::filesystem::path(name).replace_extension(".desktop"));
    ofs << desktopFile;
    ofs.close();

    std::filesystem::path home = std::getenv("HOME");
    std::filesystem::create_directories(home / ".local" / "share" / "applications");
    std::filesystem::path filePath = home / ".local" / "share" / "applications" / std::filesystem::path(name).replace_extension(".desktop");
    std::ofstream ofs1(filePath);
    ofs1 << desktopFile;
    ofs1.close();
    std::cout << home / ".local" / "share" / "applications";
//#endif
#elif defined (__APPLE__)
    makeMacGlobals();

    std::filesystem::create_directories(MACRESOURCES);
    std::filesystem::create_directories(MACAPPRESOURCES);
    std::filesystem::create_directories(MACCONTENTS);
    std::filesystem::create_directories(MACCONTENTSMACOS);
    std::filesystem::create_directories(MACCONTENTSRESOURCES);

    std::ofstream plist(MACCONTENTS / "Info.plist");
    plist << R"(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>)";
    plist << "<key>CFBundleName</key>\n";
    plist << "<string>" + noExeAppName + "</string>\n";

    plist << "<key>CFBundleIdentifier</key>\n";
    plist << "<string>" << "com.example." << noExeAppName + "</string>\n";

    plist << "<key>CFBundleVersion</key>\n";
    plist << "<string>" + version + "</string>\n";

    plist << "<key>CFBundleShortVersionString</key>\n";
    plist << "<string>" + version + "</string>\n";

    plist << "<key>CFBundleExecutable</key>\n";
    plist << "<string>" + noExeAppName + "</string>\n";

    plist << "<key>CFBundleIconFile</key>\n";
    plist << "<string>" << "icon.icns" << "</string>\n";

    plist << R"(
</dict>
</plist>)";

    std::filesystem::copy_file(MACRESOURCES / "FORGE.icns", MACCONTENTSRESOURCES / "icon.icns", std::filesystem::copy_options::overwrite_existing);
    std::filesystem::copy_file(noExeAppName, MACCONTENTSMACOS / noExeAppName, std::filesystem::copy_options::overwrite_existing);
    std::filesystem::permissions(MACCONTENTSMACOS / noExeAppName, std::filesystem::perms::owner_exec | std::filesystem::perms::group_exec | std::filesystem::perms::others_exec, std::filesystem::perm_options::add);
#endif
}