#pragma once

#include "window.hpp"
#include "sdl.hpp"

#include <array>
#include <map>
#include <memory>

enum class TextureId {
    // Game field tiles
    Airplane,
    Baloon,
    Cannibal,
    CannonDown,
    CannonLeft,
    CannonRight,
    CannonUp,
    Closed,
    Crocodile,
    DifficultTerrain,
    Fort,
    Grass,
    Horse,
    Ice,
    NativeWoman,
    PirateRed,
	PirateGreen,
	PirateBlue,
	PirateYellow,
    Pitfall,
    Root,
    Rum,
    Treasure,
    Water,
};

enum class FontId {
    FyodorBold,
};

using ArrowDescription = std::array<bool, 8>;

class Resources {
public:
    Resources() = default;
    explicit Resources(sdl::Renderer& renderer);

    sdl::Texture& texture(TextureId textureId);
    ttf::Font font(FontId fontId, int ptSize) const;
    sdl::Texture& arrowTexture(ArrowDescription arrows);

private:
    sdl::Renderer* _renderer = nullptr;
    std::map<TextureId, sdl::Texture> _textures;
    sdl::Surface _arrowBase;
    std::map<ArrowDescription, sdl::Texture> _arrowTextures;
};