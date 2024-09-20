#include <x/fs.hpp>

#include <x/exceptions.hpp>

#if defined(__linux__)
    #include <linux/limits.h>
    #include <unistd.h>
#elif defined(_WIN32)
    #include <Windows.h>
#endif

#include <format>
#include <fstream>

namespace x {

namespace {

template <class T>
void writeFileImpl(
    std::span<const T> contents, const std::filesystem::path& path)
{
    std::filesystem::create_directories(path.parent_path());
    auto output = std::ofstream{path, std::ios::binary};
    output.exceptions(std::ios::badbit | std::ios::failbit);
    output.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    output.close();
}

} // namespace

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

template <class T>
std::vector<T> readFile(const std::filesystem::path& path)
{
    auto input = std::ifstream{path, std::ios::binary | std::ios::ate};
    input.exceptions(std::ios::badbit | std::ios::failbit);

    auto fileSize = input.tellg();
    input.seekg(0);

    auto buffer = std::vector<T>(fileSize);
    input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
    input.close();

    return buffer;
}

template std::vector<char> readFile(const std::filesystem::path& path);
template std::vector<unsigned char> readFile(const std::filesystem::path& path);
template std::vector<std::byte> readFile(const std::filesystem::path& path);

void writeFile(
    std::span<const char> contents, const std::filesystem::path& path)
{
    writeFileImpl(contents, path);
}

void writeFile(
    std::span<const unsigned char> contents, const std::filesystem::path& path)
{
    writeFileImpl(contents, path);
}

void writeFile(
    std::span<const std::byte> contents, const std::filesystem::path& path)
{
    writeFileImpl(contents, path);
}

} // namespace x
