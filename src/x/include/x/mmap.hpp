#pragma once

#include <cstddef>
#include <filesystem>
#include <span>

namespace x {

class MemoryMap {
public:
    MemoryMap() = default;
    explicit MemoryMap(const std::filesystem::path& path);
    ~MemoryMap();

    MemoryMap(MemoryMap&& other) noexcept;
    MemoryMap& operator=(MemoryMap&& other) noexcept;

    MemoryMap(const MemoryMap&) = delete;
    MemoryMap& operator=(const MemoryMap&) = delete;

    void map(const std::filesystem::path& path);
    void clear();

    const void* addr() const;
    size_t size() const;
    std::span<const std::byte> bytes() const;

    friend void swap(MemoryMap& x, MemoryMap& y) noexcept;

private:
    void* _addr = nullptr;
    size_t _len = 0;
#if defined(_WIN32)
    void* _mappingHandle = nullptr;
#endif
};

} // namespace x