#include "resources.hpp"

#include <cmath>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include <algorithm>
#include <iostream>

namespace {

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

Resources::Resources(const std::filesystem::path& path, sdl::Renderer& renderer)
    : _mmap(path)
    , _fbData(fb::GetData(_mmap.addr()))
    , _renderer(&renderer)
{
    for (const auto& fbSprite : *_fbData->sprites()) {
        auto bytes = std::span<const std::byte>{
            reinterpret_cast<const std::byte*>(fbSprite->data()->data()),
            fbSprite->data()->size()
        };
        _textures.push_back(_renderer->loadTexture(bytes));
    }

    auto fbArrowBase =
        _fbData->sprites()->Get(std::to_underlying(r::Sprite::ArrowBase));
    auto fbArrowSpan = std::span{
        reinterpret_cast<const std::byte*>(fbArrowBase->data()->data()),
        fbArrowBase->data()->size()
    };
    _arrowBase = img::load(fbArrowSpan);
}

sdl::Texture& Resources::texture(r::Sprite spriteId)
{
    return _textures.at(std::to_underlying(spriteId));
}

ttf::Font Resources::font(r::Font fontId, int ptSize) const
{
    auto fbFont = _fbData->fonts()->Get(std::to_underlying(fontId));
    auto span = std::span{
        reinterpret_cast<const std::byte*>(fbFont->data()->data()),
        fbFont->data()->size()
    };
    return ttf::Font{span, ptSize};
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
