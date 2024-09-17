#pragma once

#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif

#include <memory>
#include <string>

namespace sdl {

class Surface;

}

namespace img {

sdl::Surface load(const std::string& file);

} // namespace img

namespace ttf {

class Font {
    friend Font openFont(const std::string& file, int ptsize);

public:
    struct Size {
        int w;
        int h;
    };

    Font();

    Size sizeUtf8(const std::string& text);

    sdl::Surface renderUtf8Solid(const std::string& text, const SDL_Color& fg);
    sdl::Surface renderUtf8Shaded(
        const std::string& text, const SDL_Color& fg, const SDL_Color& bg);
    sdl::Surface renderUtf8Blended(const std::string& text, const SDL_Color& fg);

private:
    Font(TTF_Font* ptr);

    std::shared_ptr<TTF_Font> _font;
};

Font openFont(const std::string& file, int ptsize);

} // namespace ttf

namespace sdl {

class Surface {
    friend class Renderer;
    friend class ttf::Font;
    friend Surface img::load(const std::string& file);

public:
    Surface();
    Surface(const Surface& other);
    ~Surface();
    
    Surface& operator=(const Surface& other);
    Surface& operator=(Surface&& other);
    explicit operator bool() const;

    const SDL_PixelFormat& format() const;
    int w() const;
    int h() const;
    int pitch() const;
    void* pixels();
    void* userdata();
    const SDL_Rect& clipRect() const;
    int& refcount();

    void fillRect(const SDL_Rect& rect, Uint32 color);

private:
    Surface(SDL_Surface* ptr);

    SDL_Surface* _surface;
};

class Texture {
    friend class Renderer;

public:
    Texture();

private:
    Texture(SDL_Texture* ptr);

    std::shared_ptr<SDL_Texture> _texture;
};

class Renderer {
    friend class Window;

public:
    Texture createTextureFromSurface(const Surface& surface) const;

    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void clear();
    void present();

    void fillRect(const SDL_Rect& rect);
    void copy(
        const Texture& texture,
        const SDL_Rect* srcrect,
        const SDL_Rect* dstrect);

private:
    Renderer(std::shared_ptr<SDL_Window> window, int index, Uint32 flags);

    std::shared_ptr<SDL_Window> _window;
    std::shared_ptr<SDL_Renderer> _renderer;
};

class Window {
public:
    struct Size {
        int w;
        int h;
    };

    Window(const std::string& title, int x, int y, int w, int h, Uint32 flags);

    Size size() const;

    Renderer createRenderer(int index, Uint32 flags);


private:
    std::shared_ptr<SDL_Window> _window;
};


} // namespace sdl
