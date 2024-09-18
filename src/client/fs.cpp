#include "fs.hpp"

#include "exceptions.hpp"

#if defined(__linux__)
    #include <linux/limits.h>
    #include <unistd.h>
#elif defined(_WIN32)
    #include <Windows.h>
#endif

#include <format>

namespace fs {

std::filesystem::path exePath()
{
#if defined(__linux__)
    auto path = std::string(PATH_MAX, '\0');
    if (readlink("/proc/self/exe", path.data(), path.length()) == -1) {
        throw Error{"failed to readlink /proc/self/exe"};
    }
    return path;
#elif defined(_WIN32)
    // TODO: do something with the path length
    auto path = std::string(5000, '\0');
    if (GetModuleFileNameA(NULL, path.data(), (DWORD)path.length()) == 0) {
        throw Error{std::format(
            "failed to GetModuleNameA: {}", GetLastError())};
    }
    return path;
#endif
}

std::filesystem::path exeDir()
{
    return exePath().parent_path();
}

} // namespace fs
