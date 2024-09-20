#pragma once

#include "window.hpp"
#include "sdl.hpp"

#include <resource-ids.hpp>
#include <x.hpp>
#include <schema_generated.h>

#include <array>
#include <filesystem>
#include <map>
#include <memory>
#include <vector>

using ArrowDescription = std::array<bool, 8>;

class Resources {
public:
    Resources() = default;
    Resources(const std::filesystem::path& path, sdl::Renderer& renderer);

    sdl::Texture& texture(r::Sprite spriteId);
    ttf::Font font(r::Font fontId, int ptSize) const;
    sdl::Texture& arrowTexture(ArrowDescription arrows);

private:
    x::MemoryMap _mmap;
    const fb::Data* _fbData = nullptr;
    sdl::Renderer* _renderer = nullptr;
    std::vector<sdl::Texture> _textures;
    sdl::Surface _arrowBase;
    std::map<ArrowDescription, sdl::Texture> _arrowTextures;
};