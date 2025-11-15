#include <filesystem>
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <limits.h>
#include <unistd.h>
#endif

std::filesystem::path getExecFolder() {
#if defined(_WIN32)
    char path[MAX_PATH];
    HMODULE hModule = GetModuleHandle(nullptr);
    if (hModule && GetModuleFileNameA(hModule, path, MAX_PATH)) {
        return std::filesystem::path(path);
    }
#elif defined(__linux__)
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count != -1) {
        return std::filesystem::path(std::string(path, count));
    }
#endif
    return {};
}