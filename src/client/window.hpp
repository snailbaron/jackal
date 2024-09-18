#pragma once

#include "geometry.hpp"
#include "sdl.hpp"
#include <memory>
#include <string>
#include <tuple>
#include <iterator>

/**
* Parameters of the game window.
*
* Can be loaded from JSON configuration file.
*/
struct WindowConfiguration {
    std::string title = "Jackal";
    int width = 800;
    int height = 600;
    bool fullscreen = false;
};


/**
* Color type used in configuration.
*/
struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    Color() : r(0), g(0), b(0), a(0) {}
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255) : r(r), g(g), b(b), a(a) {}

    template <class Container = std::initializer_list<Uint8>>
    Color(const Container& container)
        : a(255)
    {
        auto it = std::begin(container);
        r = *it++;
        g = *it++;
        b = *it++;
        if (it != container.end()) {
            a = *it;
        }
    }

    friend bool operator<(const Color& left, const Color& right)
    {
        return std::tie(left.r, left.g, left.b) <
            std::tie(right.r, right.g, right.b);
    }

    SDL_Color sdl() const { return SDL_Color{ r, g, b, a }; }
};

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
    Window(const WindowConfiguration& windowConfig);

    sdl::Texture createTextureFromSurface(sdl::Surface& surface);

    int width() const;
    int height() const;
    ScreenVector size() const;

    //
    // Graphical primitive drawing
    //

    // Fill screen with solid color
    void clear(const Color& color);

    // Draw colored axis-aligned rectangle
    void drawRect(int x, int y, int w, int h, const Color& color);
    void drawRect(const ScreenRect& rect, const Color& color);

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
        const Color& color);

    void present();

private:
    void setColor(const Color& color);

    sdl::Window _window;
    sdl::Renderer _renderer;
};
