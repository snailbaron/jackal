#include "sdl.hpp"

#include <x.hpp>

#include <format>

namespace sdl {
namespace {

template <class T>
T* check(T* ptr)
{
    if (ptr == nullptr) {
        throw x::Error{std::format("SDL: {}", SDL_GetError())};
    }
    return ptr;
}

void check(int returnValue)
{
    if (returnValue != 0) {
        throw x::Error{std::format("SDL: {}", SDL_GetError())};
    }
}

} // namespace
} // namespace sdl

namespace img {
namespace {

template <class T>
T* check(T* ptr)
{
    if (ptr == nullptr) {
        throw x::Error{std::format("SDL_image: {}", IMG_GetError())};
    }
    return ptr;
}

} // namespace
} // namespace img

namespace ttf {
namespace {

template <class T>
T* check(T* ptr)
{
    if (ptr == nullptr) {
        throw x::Error{std::format("SDL_ttf: {}", TTF_GetError())};
    }
    return ptr;
}

void check(int returnValue)
{
    if (returnValue != 0) {
        throw x::Error{std::format("SDL_ttf: {}", TTF_GetError())};
    }
}

} // namespace
} // namespace ttf

namespace sdl {

Init::Init(uint32_t flags)
{
    check(SDL_Init(flags));
}

Init::~Init()
{
    SDL_Quit();
}

Surface::Surface(const Surface& other)
    : sdl::internal::Holder<SDL_Surface, SDL_FreeSurface>(nullptr)
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

RWops::RWops(SDL_RWops* ptr)
{
    _ptr.reset(ptr);
}

RWops::RWops(const std::filesystem::path& file, const std::string& mode)
{
    _ptr.reset(check(SDL_RWFromFile(file.string().c_str(), mode.c_str())));
}

RWops::RWops(std::span<const std::byte> mem)
{
    _ptr.reset(check(SDL_RWFromConstMem(mem.data(), (int)mem.size())));
}

RWops::RWops(std::span<std::byte> mem)
{
    _ptr.reset(check(SDL_RWFromMem(mem.data(), (int)mem.size())));
}

SDL_RWops* RWops::ptr()
{
    return _ptr.get();
}

const SDL_RWops* RWops::ptr() const
{
    return _ptr.get();
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

Texture Renderer::loadTexture(const std::filesystem::path& file)
{
    return Texture{img::check(IMG_LoadTexture(ptr(), file.string().c_str()))};
}

Texture Renderer::loadTexture(std::span<const std::byte> mem)
{
    auto rw = RWops{mem};
    return Texture{img::check(IMG_LoadTexture_RW(ptr(), rw.ptr(), 0))};
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

Init::Init(int flags)
{
    if (IMG_Init(flags) != flags) {
        throw x::Error{std::format("SDL_image: {}", IMG_GetError())};
    }
}

Init::~Init()
{
    IMG_Quit();
}

sdl::Surface load(const std::filesystem::path& file)
{
    return sdl::Surface{check(IMG_Load(file.string().c_str()))};
}

sdl::Surface load(std::span<const std::byte> mem)
{
    auto rw = sdl::RWops{mem};
    return sdl::Surface{check(IMG_Load_RW(rw.ptr(), 0))};
}

} // namespace img

namespace ttf {

Init::Init()
{
    check(TTF_Init());
}

Init::~Init()
{
    TTF_Quit();
}

Font::Font(const std::filesystem::path& path, int ptsize)
{
    _ptr.reset(check(TTF_OpenFont(path.string().c_str(), ptsize)));
}

Font::Font(std::span<const std::byte> mem, int ptsize)
{
    // SDL_RWops needs to stay alive until the font is closed. The simplest way
    // to achieve that is to utilize SDL's built-in cleanup mechanism, the
    // freesrc of TTF_OpenFontRW.
    SDL_RWops* rwops = check(SDL_RWFromConstMem(mem.data(), (int)mem.size()));
    _ptr.reset(check(TTF_OpenFontRW(rwops, 1, ptsize)));
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
