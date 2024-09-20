#pragma once

#include <cstddef>
#include <filesystem>
#include <span>
#include <vector>

namespace x {

std::filesystem::path exePath();
std::filesystem::path exeDir();

// readFile has to be a template: you need a way to explicitly request a certain
// type for bytes
template <class T>
std::vector<T> readFile(const std::filesystem::path& path);

extern template std::vector<char> readFile(const std::filesystem::path& path);
extern template std::vector<unsigned char> readFile(const std::filesystem::path& path);
extern template std::vector<std::byte> readFile(const std::filesystem::path& path);

// writeFile can be a function overload
void writeFile(
    std::span<const char> contents, const std::filesystem::path& path);
void writeFile(
    std::span<const unsigned char> contents, const std::filesystem::path& path);
void writeFile(
    std::span<const std::byte> contents, const std::filesystem::path& path);

} // namespace x
