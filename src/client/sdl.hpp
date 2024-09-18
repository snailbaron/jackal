#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
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

} // namespace internal

class Init {
public:
    explicit Init(uint32_t flags);
    ~Init();

    Init(const Init&) = delete;
    Init(Init&&) = delete;
    Init& operator=(const Init&) = delete;
    Init& operator=(Init&&) = delete;
};

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

class RWops {
public:
    RWops() = default;
    explicit RWops(SDL_RWops* ptr);
    RWops(const std::filesystem::path& file, const std::string& mode);
    RWops(std::span<const std::byte> mem);
    RWops(std::span<std::byte> mem);

    SDL_RWops* ptr();
    const SDL_RWops* ptr() const;

private:
    std::unique_ptr<SDL_RWops, int(*)(SDL_RWops*)> _ptr{nullptr, SDL_RWclose};
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

    Texture loadTexture(const std::filesystem::path& file);
    Texture loadTexture(std::span<const std::byte> mem);

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

class Init {
public:
    explicit Init(int flags);
    ~Init();

    Init(const Init&) = delete;
    Init(Init&&) = delete;
    Init& operator=(const Init&) = delete;
    Init& operator=(Init&&) = delete;
};

sdl::Surface load(const std::filesystem::path& file);

} // namespace img

namespace ttf {

class Init {
public:
    Init();
    ~Init();

    Init(const Init&) = delete;
    Init(Init&&) = delete;
    Init& operator=(const Init&) = delete;
    Init& operator=(Init&&) = delete;
};

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
