#include "window.hpp"
#include <utility>

Window::Window(const Config::Window& windowConfig)
    : _window(
        windowConfig.title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        windowConfig.width,
        windowConfig.height,
        SDL_WINDOW_RESIZABLE |
            (windowConfig.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0))
    , _renderer(
        _window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
{ }

sdl::Texture Window::createTextureFromSurface(sdl::Surface& surface)
{
    return _renderer.createTextureFromSurface(surface);
}

int Window::width() const
{
    return _window.size().w;
}

int Window::height() const
{
    return _window.size().h;
}

ScreenVector Window::size() const
{
    return { _window.size().w, _window.size().h };
}

void Window::clear(const Color& color)
{
    setColor(color);
    _renderer.clear();
}

void Window::drawRect(int x, int y, int w, int h, const Color& color)
{
    setColor(color);
    _renderer.fillRect({ x, y, w, h });
}

void Window::drawRect(const ScreenRect& rect, const Color& color)
{
    setColor(color);
    _renderer.fillRect(rect.sdl());
}

void Window::drawTexture(sdl::Texture& texture, int x, int y, int w, int h)
{
    SDL_Rect dstRect{ x, y, w, h };
    _renderer.copy(texture, nullptr, &dstRect);
}

void Window::drawTexture(sdl::Texture& texture, ScreenRect rect)
{
    SDL_Rect dstRect { rect.origin.x, rect.origin.y, rect.size.x, rect.size.y };
    _renderer.copy(texture, nullptr, &dstRect);
}

void Window::drawTexturePart(
    sdl::Texture& texture, int x, int y, int w, int h, int texX, int texY, int texW, int texH)
{
    SDL_Rect dstRect { x, y, w, h };
    SDL_Rect srcRect { texX, texY, texW, texH };
    _renderer.copy(texture, &srcRect, &dstRect);
}

void Window::drawTexturePart(
    sdl::Texture& texture,
    const ScreenRect& target,
    const ScreenRect& src)
{
    SDL_Rect sdlDst = target.sdl();
    SDL_Rect sdlSrc = src.sdl();
    _renderer.copy(texture, &sdlSrc, &sdlDst);
}

void Window::drawGrid(
    int x,
    int y,
    int gridWidth,
    int gridHeight,
    int cellWidth,
    int cellHeight,
    int gridLineWidth,
    const Color& color)
{
    int verticalDelta = gridLineWidth + cellHeight;
    int horizontalDelta = gridLineWidth + cellWidth;

    int fullHeight = verticalDelta * gridHeight + gridLineWidth;
    int fullWidth = horizontalDelta * gridWidth + gridLineWidth;

    // Draw horizontal lines
    for (int i = 0; i <= gridHeight; i++) {
        drawRect(x, y + i * verticalDelta, fullWidth, gridLineWidth, color);
    }

    // Draw vertical lines
    for (int i = 0; i <= gridWidth; i++) {
        drawRect(x + i * horizontalDelta, y, gridLineWidth, fullHeight, color);
    }
}

void Window::present()
{
    _renderer.present();
}

/*
 * Implementation details
 */

void Window::setColor(const Color& color)
{
    _renderer.setDrawColor(color.r, color.g, color.b, color.a);
}