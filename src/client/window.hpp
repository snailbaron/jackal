#pragma once

#include "config.hpp"
#include "geometry.hpp"
#include "sdl.hpp"
#include <memory>
#include <string>
#include <tuple>
#include <iterator>

/**
 * Window for drawing graphical primitives in immediate mode.
 *
 * A Window incapsulates SDL_Window and SDL_Renderer. It is able to draw simple
 * graphical primitives in immediate mode. To use a window, you usually call
 * several 'draw...' functions, then present(), to show the result on screen:
 *     window.draw...
 *     window.draw...
 *     window.draw...
 *     window.present();
 * This sequence is typically performed each frame.
 *
 * Some of the 'draw...' calls wrap SDL rendering functions. Others provide
 * convenient ways to draw game-specific primitives.
 */
class Window {
public:
    Window() {}
    Window(const Config::Window& windowConfig);

    sdl::Texture createTextureFromSurface(sdl::Surface& surface);

    int width() const;
    int height() const;
    ScreenVector size() const;

    sdl::Window& window();
    sdl::Renderer& renderer();

    //
    // Graphical primitive drawing
    //

    // Fill screen with solid color
    void clear(const SDL_Color& color);
    void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    // Draw colored axis-aligned rectangle
    void drawRect(int x, int y, int w, int h, const SDL_Color& color);
    void drawRect(const ScreenRect& rect, const SDL_Color& color);

    // Draw a whole texture on screen
    void drawTexture(sdl::Texture& texture, int x, int y, int w, int h);
    void drawTexture(sdl::Texture& texture, ScreenRect rect);

    // Draw a part of texture on screen
    void drawTexturePart(
        sdl::Texture& texture,
        int x, int y, int w, int h,
        int texX, int texY, int texW, int texH);
    void drawTexturePart(
        sdl::Texture& texture,
        const ScreenRect& target,
        const ScreenRect& src);

    void drawGrid(
        int x,
        int y,
        int gridWidth,
        int gridHeight,
        int cellWidth,
        int cellHeight,
        int gridLineWidth,
        const SDL_Color& color);

    void present();

private:
    void setColor(const SDL_Color& color);
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    sdl::Window _window;
    sdl::Renderer _renderer;
};
