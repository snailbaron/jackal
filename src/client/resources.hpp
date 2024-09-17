#pragma once

#include "window.hpp"
#include "sdl_wrapper.hpp"
#include <memory>
#include <map>
#include <array>

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
    Resources(std::shared_ptr<const Window> window);

    sdl::Texture texture(TextureId textureId) const;
    ttf::Font font(FontId fontId, int ptSize) const;
    sdl::Texture arrowTexture(ArrowDescription arrows) const;

private:
    std::shared_ptr<const Window> _window;
    std::map<TextureId, sdl::Texture> _textures;
    sdl::Surface _arrowBase;
};