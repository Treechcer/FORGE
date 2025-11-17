#pragma once

#include <filesystem>
#include <iostream>
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <limits.h>
#include <unistd.h>
#include <pwd.h>
#endif

std::filesystem::path getExecFolder();
std::filesystem::path getConfigPath();
std::string getDefaultClangFile();