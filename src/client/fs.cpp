#include "fs.hpp"

#include "exceptions.hpp"

#if defined(__linux__)
    #include <linux/limits.h>
    #include <unistd.h>
#elif defined(_WIN32)
#endif

namespace fs {

std::filesystem::path exeDir()
{
#if defined(__linux__)
    auto path = std::string(PATH_MAX, '\0');
    if (readlink("/proc/self/exe", path.data(), path.length()) == -1) {
        throw Error{"failed to readlink /proc/self/exe"};
    }
    return path;
#elif defined(_WIN32)
#endif
}

} // namespace fs
