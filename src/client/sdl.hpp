#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

namespace sdl {

namespace internal {

template <class T, void(*deleter)(T*)>
class Holder {
public:
    Holder() = default;

    explicit Holder(T* ptr)
    {
        _ptr.reset(ptr);
    }

    explicit operator bool() const noexcept
    {
        return !!_ptr;
    }

    const T* ptr() const noexcept
    {
        return _ptr.get();
    }

    T* ptr() noexcept
    {
        return _ptr.get();
    }

    const T* operator->() const noexcept
    {
        return _ptr.get();
    }

    T* operator->() noexcept
    {
        return _ptr.get();
    }

protected:
    std::unique_ptr<T, void(*)(T*)> _ptr{nullptr, deleter};
};

} // namespace

class Surface : public internal::Holder<SDL_Surface, SDL_FreeSurface> {
public:
    using internal::Holder<SDL_Surface, SDL_FreeSurface>::Holder;

    Surface(const Surface& other);
    Surface& operator=(const Surface& other);

    void fillRect(const SDL_Rect& rect, Uint32 color);
};

class Texture : public internal::Holder<SDL_Texture, SDL_DestroyTexture> {
public:
    using internal::Holder<SDL_Texture, SDL_DestroyTexture>::Holder;
};

class Window : public internal::Holder<SDL_Window, SDL_DestroyWindow> {
public:
    struct Size {
        int w;
        int h;
    };

    Window() = default;
    Window(const std::string& title, int x, int y, int w, int h, Uint32 flags);

    Size size() const;
};

class Renderer : public internal::Holder<SDL_Renderer, SDL_DestroyRenderer> {
public:
    Renderer() = default;
    Renderer(Window& window, int index, uint32_t flags);

    Texture createTextureFromSurface(Surface& surface);

    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void clear();
    void present();

    void fillRect(const SDL_Rect& rect);
    void copy(
        Texture& texture,
        const SDL_Rect* srcrect,
        const SDL_Rect* dstrect);
};

} // namespace sdl

namespace img {

sdl::Surface load(const std::string& file);

} // namespace img

namespace ttf {

class Font : public sdl::internal::Holder<TTF_Font, TTF_CloseFont> {
public:
    struct Size {
        int w;
        int h;
    };

    Font(const std::filesystem::path& path, int ptsize);

    Size sizeUtf8(const std::string& text);

    sdl::Surface renderUtf8Solid(const std::string& text, const SDL_Color& fg);
    sdl::Surface renderUtf8Shaded(
        const std::string& text, const SDL_Color& fg, const SDL_Color& bg);
    sdl::Surface renderUtf8Blended(const std::string& text, const SDL_Color& fg);
};

} // namespace ttf
