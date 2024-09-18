#include "fs.hpp"

#include "exceptions.hpp"

#ifdef __linux__
    #include <linux/limits.h>
    #include <unistd.h>
#elifdef _WIN32
#endif

namespace fs {

std::filesystem::path exeDir()
{
#ifdef __linux__
    auto path = std::string(PATH_MAX, '\0');
    if (readlink("/proc/self/exe", path.data(), path.length()) == -1) {
        throw Error{"failed to readlink /proc/self/exe"};
    }
    return path;
#elifdef _WIN32
#endif
}

} // namespace fs
