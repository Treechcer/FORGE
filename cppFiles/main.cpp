#include "../headers/configParser.h"
#include "../headers/constants.h"
#include "../headers/funcs.h"
#include "../headers/inputFuncs.h"
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <limits.h>
#include <unistd.h>
#endif

void compileOne(std::filesystem::path pathAfter);
void buildPorject(std::vector<std::filesystem::path> pathAfter);
void creatingProject(bool writeOutEnd);

std::vector<std::thread> threads;
std::vector<std::filesystem::path> pathToCompile;

bool HASH = false;
std::filesystem::path OUTPUTPATH = "";
int THREADNUMBER = 4;

size_t hashString(std::string toHash) {
    std::string text = toHash;
    std::hash<std::string> hasher;
    size_t hashValue = hasher(text);
    //std::cout << hashValue << std::endl;
    return hashValue;
}

void compileN(std::vector<std::filesystem::path> pathAfter) {
    for (int i = 0; i < pathAfter.size(); i++) {

        if (pathAfter[i].extension() == ".h") {
            continue;
        }

        std::string cmd = COMPILERCOMMAND + " -c ";
        cmd.append(pathAfter[i].string());
        cmd.append(" -o ");
        cmd.append(pathAfter[i].replace_extension(".o").string());
        //std::cout << cmd << std::endl;
        int res = system(cmd.c_str());
        if (res != 0) {
            std::cout << "Compilation failed, output path: " << pathAfter[i].string();
            exit(0);
        }

        std::cout << "Executin command:" << cmd << std::endl;
    }
}

void compileWithThread() {
    int filePerThread = std::ceil((double)pathToCompile.size() / THREADNUMBER);
    //std::cout << filePerThread;
    std::vector<std::filesystem::path> tempFiles;
    for (int i = 0; i < pathToCompile.size(); i++) {
        tempFiles.push_back(pathToCompile[i]);
        if ((tempFiles.size()) % filePerThread == 0) {
            //std::thread t (compileN, tempFiles);
            threads.push_back(std::thread(compileN, std::vector<std::filesystem::path>(tempFiles)));
            tempFiles.clear();
        }
    }

    if (!tempFiles.empty()) {
        threads.push_back(std::thread(compileN, std::vector<std::filesystem::path>(tempFiles)));
        tempFiles.clear();
    }

    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
}

std::vector<std::filesystem::path> getFiles(std::filesystem::path dir, std::vector<std::filesystem::path> paths) {
    if (std::regex_match(dir.string(), std::regex(".*\\.git")) || std::regex_match(dir.string(), std::regex(".*\\.FORGE"))) {
        return paths;
    }
    for (const auto &entry : std::filesystem::directory_iterator(dir)) {
        std::string str = entry.path().string();
        //std::cout << str << std::endl;
        if (std::regex_match(str, std::regex(".*\\.cpp$")) || std::regex_match(str, std::regex(".*\\.h$"))) {
            //std::cout << entry.path() << "\n";
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

void copyHeaderFiles(std::vector<std::filesystem::path> pathBefore, std::vector<std::filesystem::path> pathAfter) {
    for (int i = 0; i < pathAfter.size(); i++) {
        if (!(pathAfter[i].extension() == ".h")) {
            continue;
        }
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
            else if (std::filesystem::last_write_time(pathBefore[i]) > std::filesystem::last_write_time(pathAfter[i])) {
                copy = true;
            }
        }

        if (copy) {
            std::filesystem::create_directories(pathAfter[i].parent_path());
            std::filesystem::copy(pathBefore[i], pathAfter[i], std::filesystem::copy_options::overwrite_existing);

            pathToCompile.push_back(pathAfter[i]);
            //compileOne(pathAfter[i]);
        }
    }
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
            else if (std::filesystem::last_write_time(pathBefore[i]) > std::filesystem::last_write_time(pathAfter[i])) {
                copy = true;
            }
        }

        if (copy) {
            std::filesystem::create_directories(pathAfter[i].parent_path());
            std::filesystem::copy(pathBefore[i], pathAfter[i], std::filesystem::copy_options::overwrite_existing);

            pathToCompile.push_back(pathAfter[i]);
            //compileOne(pathAfter[i]);
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

        std::string cmd = COMPILERCOMMAND + " -c ";
        cmd.append(pathAfter[i].string());
        cmd.append(" -o ");
        cmd.append(pathAfter[i].replace_extension(".o").string());
        //std::cout << cmd << std::endl;
        int res = system(cmd.c_str());
        if (res != 0) {
            std::cout << "Compilation failed, on command: " << cmd;
            exit(0);
        }
    }
}

void compileOne(std::filesystem::path pathAfter) {
    if (pathAfter.extension() == ".h") {
        return;
    }

    std::string cmd = COMPILERCOMMAND + " -c ";
    cmd.append(pathAfter.string());
    cmd.append(" -o ");
    cmd.append(pathAfter.replace_extension(".o").string());
    //std::cout << cmd << std::endl;
    int res = system(cmd.c_str());
    if (res != 0) {
        std::cout << "Compilation failed, output path: " << pathAfter.string();
        exit(0);
    }
}

bool strToBool(std::string strBool) {
    if (strBool == "true")
        return true;
    return false;
}

void buildPorject(std::vector<std::filesystem::path> pathAfter, std::filesystem::path OUTPUTPATH) {
    std::string allObJs = "";
    for (int i = 0; i < pathAfter.size(); i++) {
        if (pathAfter[i].extension() == ".h") {
            continue;
        }
        allObJs.append(pathAfter[i].replace_extension(".o").string() + " ");
    }

#ifdef _WIN32
    std::filesystem::path execFolder = getExecFolder();
    std::filesystem::path rcFile = execFolder.parent_path() / "windowsResources" / "resources.rc";
    std::filesystem::path objFile = (std::filesystem::path) FORGEPROJECTPATH / "resources.o";

    if (std::filesystem::exists(rcFile)) {
        //std::cout << execFolder << "\n" << rcFile << "\n" << objFile << "\n";
        std::string cmd = "windres ";
        cmd.append(rcFile.string() + " ");
        cmd.append(objFile.string());
        //std::cout << cmd;
        int res = system(cmd.c_str());
        if (res != 0) {
            std::cout << "Compilation failed, output path: " << rcFile;
            exit(0);
        }
        allObJs.append(objFile.string() + " ");
    }
#endif

    //std::cout << allObJs << "\n";
    std::string appName = cfgVals("exeName");
    if (!appName.empty() && appName.back() == '\r') {
        appName.pop_back();
    }
#if defined(__linux__) || defined(__APPLE__)
    appName = std::regex_replace(appName, std::regex("\\.exe$"), "");
    appName = std::regex_replace(appName, std::regex("\\.exe'$"), "");
    appName = std::regex_replace(appName, std::regex("\\.exe\"$"), "");
#elif _WIN32
    if (!std::regex_search(appName, std::regex("\\.exe$")) && !std::regex_search(appName, std::regex("\\.exe\"$")) && !std::regex_search(appName, std::regex("\\.exe'$"))) {
        appName.append(".exe");
    }
#endif

    std::string cmd = COMPILERCOMMAND + " " + allObJs + "-o ";
    cmd.append((OUTPUTPATH / appName).string());
    //std::cout << cmd;
    int res = system(cmd.c_str());
    if (res != 0) {
        std::cout << "Compilation failed, on command : " << cmd;
        exit(0);
    }
    //std::cout << cmd << std::endl;
}

int checkInputs(int argc, char *argv[], std::filesystem::path currentDir) {
    std::filesystem::path execFolder = getExecFolder();
    for (int i = 0; i < argc; i++) {
        //std::cout << argv[i] << " " << i << std::endl;
        std::string cmd = argv[i];
        if (cmd == "-update") {
            //std::filesystem::current_path(execFolder);
            update(COMPILERCOMMAND, argv[0]);
            //std::filesystem::current_path(currentDir);
            i++;
        }
        else if (cmd == "-path") {
            OUTPUTPATH = argv[i + 1];
            i++;
        }
        else if (cmd == "-timeTest") {

            if (i + 1 >= argc) {
                std::cout << "you have to have another numeric argument after -timeTest";
                return 1;
            }

            if (!(std::stoi(argv[i + 1]) > 0)) {
                std::cout << "-timeTest has to have number after it";
                return 1;
            }

            std::filesystem::remove_all(FORGEPROJECTPATH);

            std::string nameBefore = parser::variableValueCreator("exeName");
            parser::variableRewrite("exeName", "timeTest");
            std::cout << "TimeTest Start now, max threads " << THREADNUMBER << std::endl;

            std::vector<int> timesMS;

            for (int j = 0; j < std::stoi(argv[i + 1]); j++) {
                auto now = std::chrono::system_clock::now();
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

                creatingProject(false);

                auto endTime = std::chrono::system_clock::now();
                auto msEnd = std::chrono::duration_cast<std::chrono::milliseconds>(endTime.time_since_epoch()).count();
                std::cout << "time (ms): " << (msEnd - ms) << std::endl;
                std::cout << "time (s): " << double(msEnd - ms) / 1000 << std::endl;
                //std::cout << "compiled times: " << j + 1 << std::endl;

                int totalRuns = std::stoi(argv[i + 1]);
                int done = j + 1;

                int made = (done * 10) / totalRuns;
                int notMade = 10 - made;
                std::string outBar = "[";
                outBar += std::string(made, '#');
                outBar += std::string(notMade, '-');
                outBar.append("]");

                std::cout << std::ceil(((double)done / totalRuns * 100) * 100) / 100 << outBar << std::endl;

                timesMS.push_back(msEnd - ms);
                if (j < std::stoi(argv[i + 1]) - 1) {
                    std::filesystem::remove_all(FORGEPROJECTPATH);
                    std::filesystem::create_directory(FORGEPROJECTPATH);
                }
                threads.clear();
            }

            int sum = 0;
            for (int j = 0; j < timesMS.size(); j++) {
                sum += timesMS[j];
            }
            double avg = sum / std::stoi(argv[i + 1]);
            std::cout << "average time (ms): " << avg << std::endl;
            std::cout << "average time (s): " << std::ceil((avg / 1000) * 100) / 100 << std::endl;
            std::cout << "total time (ms): " << sum << std::endl;
            std::cout << "total time (s): " << std::ceil(((double)sum / 1000) * 100) / 100 << std::endl;
            parser::variableRewrite("exeName", nameBefore);

            std::filesystem::remove("timeTest.exe");

            return 1;
        }
        else if (cmd == "-thread") {
            if (std::stoi(argv[i + 1]) < 1) {
                std::cout << "You can't have less than 1 threads";
                return 1;
            }
            THREADNUMBER = std::stoi(argv[i + 1]);
        }
    }

    return 0;
}

void createDirs() {
    std::filesystem::create_directories(FORGEPATH);
    std::filesystem::create_directories(FORGEPROJECTPATH);
    std::filesystem::create_directories(FORGEDATAPATH);
}

void creatingProject(bool writeOutEnd = true) {
    pathToCompile.clear();

    std::vector<std::filesystem::path> paths;
    std::vector<std::filesystem::path> changedPaths;
    std::filesystem::path thisDir = ".";
    paths = getFiles(thisDir, paths);

    changedPaths = changePaths(paths);
    copyHeaderFiles(paths, changedPaths);
    int status = copyFiles(paths, changedPaths);

    if (status == 1) {
        return;
    }

    //for (int i = 0; i < pathToCompile.size(); i++) {
    //    std::cout << pathToCompile[i] << std::endl;
    //}

    //compileAll(changedPaths);

    compileWithThread();

    buildPorject(changedPaths, OUTPUTPATH);

    //for (int i = 0; i < changedPaths.size(); i++){
    //    std::cout << changedPaths[i].extension();
    //}
    if (writeOutEnd) {
        std::cout << "You code was compiled into: " << cfgVals("exeName") << std::endl;
    }
}

int main(int argc, char *argv[]) {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    //std::cout << "start: " << ms << std::endl;

    //std::cout << std::filesystem::current_path() << std::endl;

    createDirs();

    std::filesystem::path currentDir = std::filesystem::current_path();

    std::filesystem::path execFolder = getExecFolder();

    CONFIGFOLDER = getConfigPath();

    //std::cout << CONFIGFOLDER;

    std::filesystem::create_directories(CONFIGFOLDER);

    if (!std::filesystem::exists(CONFIGFOLDER / "forge.forgecfg")) {
        std::ofstream ofs(CONFIGFOLDER / "forge.forgecfg");
        ofs << parser::defaultConfig();
    }

    if (!std::filesystem::exists(FORGEDATAPATH / "forge.forgecfg")) {
        std::filesystem::copy(CONFIGFOLDER / "forge.forgecfg", FORGEDATAPATH / "forge.forgecfg", std::filesystem::copy_options::overwrite_existing);
        create();
    }

    COMPILERCOMMAND = cfgVals("compileCommand");

    int inputStatus = checkInputs(argc, argv, currentDir);

    if (inputStatus == 1) {
        return 0;
    }

    std::filesystem::create_directory(FORGEPATH);
#ifdef _WIN32
    //system(("windres " + (execFolder.parent_path() / "resources.rc").string()(execFolder.parent_path() / "resources.rc").string() + "./.FORGE/PROJECT/resources.o").c_str());
    SetFileAttributesA(FORGEPATH.string().c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
#endif

    create();

    HASH = strToBool(cfgVals("hash"));

    creatingProject();

    auto endTime = std::chrono::system_clock::now();
    auto msEnd = std::chrono::duration_cast<std::chrono::milliseconds>(endTime.time_since_epoch()).count();
    std::cout << "time (ms): " << (msEnd - ms) << std::endl;
    std::cout << "time (s): " << double(msEnd - ms) / 1000 << std::endl;

    return 0;
}