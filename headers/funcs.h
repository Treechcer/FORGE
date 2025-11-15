#pragma once

#include <filesystem>
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <limits.h>
#include <unistd.h>
#endif

std::filesystem::path getExecFolder();