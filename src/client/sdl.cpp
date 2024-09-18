#include "sdl.hpp"
#include "exceptions.hpp"
#include <stdexcept>
#include <sstream>

namespace sdl {

namespace {

template <class T>
T* check(T* ptr)
{
    if (ptr == nullptr) {
        throw Error{std::format("SDL: {}", SDL_GetError())};
    }
    return ptr;
}

void check(int returnValue)
{
    if (returnValue != 0) {
        throw Error{std::format("SDL: {}", SDL_GetError())};
    }
}

} // namespace

Surface::Surface(const Surface& other)
{
    _ptr.reset(check(SDL_ConvertSurface(other._ptr.get(), other->format, 0)));
}

Surface& Surface::operator=(const Surface& other)
{
    if (this != &other) {
        _ptr.reset(check(SDL_ConvertSurface(other._ptr.get(), other->format, 0)));
    }
    return *this;
}

void Surface::fillRect(const SDL_Rect& rect, Uint32 color)
{
    check(SDL_FillRect(ptr(), &rect, color));
}

Window::Window(
    const std::string& title,
    int x,
    int y,
    int w,
    int h,
    Uint32 flags)
{
    _ptr.reset(check(SDL_CreateWindow(title.c_str(), x, y, w, h, flags)));
}

Window::Size Window::size() const
{
    Size size;
    SDL_GetWindowSize(_ptr.get(), &size.w, &size.h);
    return size;
}

Renderer::Renderer(Window& window, int index, uint32_t flags)
{
    _ptr.reset(check(SDL_CreateRenderer(window.ptr(), index, flags)));

    check(SDL_SetRenderDrawBlendMode(ptr(), SDL_BLENDMODE_BLEND));
}

Texture Renderer::createTextureFromSurface(Surface& surface)
{
    return Texture{check(
        SDL_CreateTextureFromSurface(ptr(), surface.ptr()))};
}

void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    check(SDL_SetRenderDrawColor(ptr(), r, g, b, a));
}

void Renderer::clear()
{
    check(SDL_RenderClear(ptr()));
}

void Renderer::present()
{
    SDL_RenderPresent(ptr());
}

void Renderer::fillRect(const SDL_Rect& rect)
{
    check(SDL_RenderFillRect(ptr(), &rect));
}

void Renderer::copy(
    Texture& texture,
    const SDL_Rect* srcrect,
    const SDL_Rect* dstrect)
{
    check(SDL_RenderCopy(ptr(), texture.ptr(), srcrect, dstrect));
}

} // namespace sdl

namespace img {

namespace {

template <class T>
T* check(T* ptr)
{
    if (ptr == nullptr) {
        throw Error{std::format("SDL_image: {}", IMG_GetError())};
    }
    return ptr;
}

void check(int returnValue)
{
    if (returnValue != 0) {
        throw Error{std::format("SDL_image: {}", IMG_GetError())};
    }
}

} // namespace

sdl::Surface load(const std::string& file)
{
    return sdl::Surface(check(IMG_Load(file.c_str())));
}

} // namespace img

namespace ttf {

namespace {

template <class T>
T* check(T* ptr)
{
    if (ptr == nullptr) {
        throw Error{std::format("SDL_ttf: {}", TTF_GetError())};
    }
    return ptr;
}

void check(int returnValue)
{
    if (returnValue != 0) {
        throw Error{std::format("SDL_ttf: {}", TTF_GetError())};
    }
}

} // namespace

Font::Font(const std::filesystem::path& path, int ptsize)
{
    _ptr.reset(check(TTF_OpenFont(path.string().c_str(), ptsize)));
}

Font::Size Font::sizeUtf8(const std::string& text)
{
    Size size;
    check(TTF_SizeUTF8(ptr(), text.c_str(), &size.w, &size.h));
    return size;
}

sdl::Surface Font::renderUtf8Solid(const std::string& text, const SDL_Color& fg)
{
    return sdl::Surface{
        check(TTF_RenderUTF8_Solid(ptr(), text.c_str(), fg))};
}

sdl::Surface Font::renderUtf8Shaded(
    const std::string& text, const SDL_Color& fg, const SDL_Color& bg)
{
    return sdl::Surface{
        check(TTF_RenderUTF8_Shaded(ptr(), text.c_str(), fg, bg))};
}

sdl::Surface Font::renderUtf8Blended(const std::string& text, const SDL_Color& fg)
{
    return sdl::Surface{
        check(TTF_RenderUTF8_Blended(ptr(), text.c_str(), fg))};
}

} // namespace ttf
