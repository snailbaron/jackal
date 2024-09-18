#include "resources.hpp"
#include <string>
#include <functional>
#include <cmath>
#include <vector>

namespace {

std::map<TextureId, std::string> texturePaths {
    { TextureId::Airplane,         "airplane.bmp" },
    { TextureId::Baloon,           "baloon.bmp" },
    { TextureId::Cannibal,         "cannibal.bmp" },
    { TextureId::CannonDown,       "cannon_down.bmp" },
    { TextureId::CannonLeft,       "cannon_left.bmp" },
    { TextureId::CannonRight,      "cannon_right.bmp" },
    { TextureId::CannonUp,         "cannon_up.bmp" },
    { TextureId::Closed,           "closed.bmp" },
    { TextureId::Crocodile,        "crocodile.bmp" },
    { TextureId::DifficultTerrain, "difficult_terrain.bmp" },
    { TextureId::Fort,             "fort.bmp" },
    { TextureId::Grass,            "grass.bmp" },
    { TextureId::Horse,            "horse.bmp" },
    { TextureId::Ice,              "ice.bmp" },
    { TextureId::NativeWoman,      "native_woman.bmp" },
    { TextureId::PirateRed,        "pirate_red.png" },
	{ TextureId::PirateGreen,      "pirate_green.png" },
	{ TextureId::PirateBlue,       "pirate_blue.png" },
	{ TextureId::PirateYellow,     "pirate_yellow.png" },
    { TextureId::Pitfall,          "pitfall.bmp" },
    { TextureId::Root,             "root.bmp" },
    { TextureId::Rum,              "rum.bmp" },
    { TextureId::Treasure,         "treasure.bmp" },
    { TextureId::Water,            "water.bmp" },
};

std::map<FontId, std::string> fontPaths {
    { FontId::FyodorBold, "fyodor/truetype/Fyodor-Bold.ttf" },
};

struct FontInfo {
    FontId id;
    int size;
};

// TODO: Move to some drawing wrapper over SDL wrapper?
// (wrapper in wrapper!)
void drawPixel(
    sdl::Surface& canvas, int x, int y, const Color& color)
{
    // TODO: Modify surface pixels directly, instead of filling a 1x1 rectangle
    SDL_Rect rect { x, y, 1, 1 };
    canvas.fillRect(rect, SDL_MapRGBA(canvas->format, color.r, color.g, color.b, color.a));
}

void drawCircle(
    sdl::Surface& canvas,
    const ScreenVector& center,
    int radius,
    const Color& color)
{
    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            double diff = std::sqrt(x * x + y * y) - radius;
            if (diff < 0) {
                drawPixel(canvas, center.x + x, center.y + y, color);
            } else if (diff < 1) {
                Color newColor = color;
                newColor.a = static_cast<uint8_t>(
                    std::round(color.a * (1 - diff)));
                drawPixel(canvas, center.x + x, center.y + y, newColor);
            }
        }
    }
}

void drawArrow(
    sdl::Surface& canvas,
    const ScreenVector& start,
    const ScreenVector& end,
    const Color& color)
{
    drawCircle(canvas, start, 10, color);
    drawCircle(canvas, end, 10, color);
}

sdl::Surface createArrowSurface(const sdl::Surface& base, ArrowDescription arrows)
{
    sdl::Surface canvas = base;

    std::vector<ScreenVector> directions = {
        { 15, 48 },
        { 32, 48 },
        { 48, 48 },
        { 15, 32 },
        { 48, 32 },
        { 15, 15 },
        { 32, 15 },
        { 48, 15 },
    };

    for (int i = 0; i < 8; i++) {
        if (arrows[i]) {
            drawArrow(canvas, {32, 32}, directions[i], {255, 0, 0, 255});
        }
    }
    return canvas;
}

} // namespace

using namespace std::placeholders;

Resources::Resources(Window& window)
    : _window(&window)
{
    std::string tilesPath = "assets/tiles/";

    for (const auto& texturePath : texturePaths) {
        std::string path = tilesPath + texturePath.second;

        sdl::Surface surface = img::load(path);
        sdl::Texture texture = _window->createTextureFromSurface(surface);
        _textures[texturePath.first] = std::move(texture);
    }

    _arrowBase = img::load(tilesPath + "arrow_base.png");
}

sdl::Texture& Resources::texture(TextureId textureId)
{
    return _textures.at(textureId);
}

ttf::Font Resources::font(FontId fontId, int ptSize) const
{
    std::string fontPath = "assets/fonts/" + fontPaths.at(fontId);
    return ttf::Font{fontPath, ptSize};
}

sdl::Texture& Resources::arrowTexture(ArrowDescription arrows)
{
    if (auto it = _arrowTextures.find(arrows); it != _arrowTextures.end()) {
        return it->second;
    }

    sdl::Surface surface = createArrowSurface(_arrowBase, arrows);
    sdl::Texture texture = _window->createTextureFromSurface(surface);
    return _arrowTextures.emplace(arrows, std::move(texture)).first->second;
}
