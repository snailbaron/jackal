#include "resources.hpp"

#include "fs.hpp"

#include <cmath>
#include <functional>
#include <string>
#include <vector>

namespace {

std::map<TextureId, std::string> texturePaths {
    { TextureId::Airplane,         "airplane.png" },
    { TextureId::Baloon,           "baloon.png" },
    { TextureId::Cannibal,         "cannibal.png" },
    { TextureId::CannonDown,       "cannon_down.png" },
    { TextureId::CannonLeft,       "cannon_left.png" },
    { TextureId::CannonRight,      "cannon_right.png" },
    { TextureId::CannonUp,         "cannon_up.png" },
    { TextureId::Closed,           "closed.png" },
    { TextureId::Crocodile,        "crocodile.png" },
    { TextureId::DifficultTerrain, "difficult_terrain.png" },
    { TextureId::Fort,             "fort.png" },
    { TextureId::Grass,            "grass.png" },
    { TextureId::Horse,            "horse.png" },
    { TextureId::Ice,              "ice.png" },
    { TextureId::NativeWoman,      "native_woman.png" },
    { TextureId::PirateRed,        "pirate_red.png" },
	{ TextureId::PirateGreen,      "pirate_green.png" },
	{ TextureId::PirateBlue,       "pirate_blue.png" },
	{ TextureId::PirateYellow,     "pirate_yellow.png" },
    { TextureId::Pitfall,          "pitfall.png" },
    { TextureId::Root,             "root.png" },
    { TextureId::Rum,              "rum.png" },
    { TextureId::Treasure,         "treasure.png" },
    { TextureId::Water,            "water.png" },
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

Resources::Resources(sdl::Renderer& renderer)
    : _renderer(&renderer)
{
    auto tilesPath = fs::exeDir() / "assets" / "tiles";

    for (const auto& texturePath : texturePaths) {
        auto path = tilesPath / texturePath.second;
        _textures[texturePath.first] = _renderer->loadTexture(path);
    }

    _arrowBase = img::load(tilesPath / "arrow_base.png");
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
    sdl::Texture texture = _renderer->createTextureFromSurface(surface);
    return _arrowTextures.emplace(arrows, std::move(texture)).first->second;
}
