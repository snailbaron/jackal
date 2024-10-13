#include <x/mmap.hpp>

#include <x/exceptions.hpp>

#if defined(__linux__)
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

#include <cstring>
#include <format>
#include <source_location>
#include <string_view>

namespace x {

namespace {

#if defined(__linux__)
void checkErrno(std::source_location sl = std::source_location::current())
{
    int e = errno;
    throw Error{
        std::format("{}: {}", strerrorname_np(e), strerrordesc_np(e)),
        sl
    };
}
#elif defined(_WIN32)
[[noreturn]] void throwWindowsError(std::string_view message)
{
    DWORD error = GetLastError();

    LPTSTR messageBuffer = nullptr;
    if (FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&messageBuffer,
            0,
            NULL) == 0) {
        auto formatMessageError = GetLastError();
        throw Error{std::format("format message error: {}", formatMessageError)};
    }

    auto finalMessage = std::format("{}: {}", message, messageBuffer);

    LocalFree(messageBuffer);

    throw Error{finalMessage};
}
#endif

} // namespace

MemoryMap::MemoryMap(const std::filesystem::path& path)
{
    map(path);
}

MemoryMap::~MemoryMap()
{
    clear();
}

MemoryMap::MemoryMap(MemoryMap&& other) noexcept
{
    swap(*this, other);
}

MemoryMap& MemoryMap::operator=(MemoryMap&& other) noexcept
{
    if (this != &other) {
        clear();
        swap(*this, other);
    }
    return *this;
}

void MemoryMap::map(const std::filesystem::path& path)
{
#if defined(__linux)
    int fd = open(path.string().c_str(), O_RDONLY);
    if (fd == -1) {
        int e = errno;
        throw Error{std::format(
            "cannot open file {}: {}: {}",
            path.string(), strerrorname_np(e), strerrordesc_np(e))};
    };

    auto fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1) {
        checkErrno();
    }
    _len = fileSize;

    void* addr = mmap(nullptr, fileSize, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        checkErrno();
    }
    _addr = addr;
#elif defined(_WIN32)
    HANDLE fileHandle = CreateFileW(
        path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        throwWindowsError(std::format("cannot read file '{}'", path.string()));
    }

    LARGE_INTEGER fileSize;
    if (GetFileSizeEx(fileHandle, &fileSize) == 0) {
        throwWindowsError("cannot get file size");
    }
    _len = fileSize.QuadPart;

    HANDLE mappingHandle = CreateFileMapping(
        fileHandle,
        NULL,
        PAGE_READONLY,
        0,
        0,
        NULL);
    if (mappingHandle == NULL) {
        throwWindowsError("cannot create file mapping");
    }
    _mappingHandle = mappingHandle;

    if (CloseHandle(fileHandle) == 0) {
        throwWindowsError("cannot close file handle");
    }

    LPVOID addr = MapViewOfFile(
        mappingHandle,
        FILE_MAP_READ,
        0,
        0,
        0);
    if (addr == NULL) {
        throwWindowsError("cannot create file map view");
    }
    _addr = addr;
#endif
}

void MemoryMap::clear()
{
    if (_addr) {
#if defined(__linux__)
        munmap(_addr, _len);
#elif defined(_WIN32)
        UnmapViewOfFile(_addr);
        CloseHandle(_mappingHandle);
#endif
    }
    _addr = nullptr;
    _len = 0;
}

const void* MemoryMap::addr() const
{
    return _addr;
}

size_t MemoryMap::size() const
{
    return _len;
}

std::span<const std::byte> MemoryMap::bytes() const
{
    return {reinterpret_cast<const std::byte*>(addr()), size()};
}

void swap(MemoryMap& x, MemoryMap& y) noexcept
{
    std::swap(x._addr, y._addr);
    std::swap(x._len, y._len);
#ifdef _WIN32
    std::swap(x._mappingHandle, y._mappingHandle);
#endif
}

} // namespace x
