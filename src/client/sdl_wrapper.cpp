#include "sdl_wrapper.hpp"
#include "exceptions.hpp"
#include <stdexcept>
#include <sstream>

namespace ttf {

Font::Font()
    : _font(nullptr)
{ }

Font::Font(TTF_Font* ptr)
    : _font(ptr, TTF_CloseFont)
{ }

Font::Size Font::sizeUtf8(const std::string& text)
{
    Size size;
    TTF_SizeUTF8(_font.get(), text.c_str(), &size.w, &size.h);
    return size;
}

sdl::Surface Font::renderUtf8Solid(const std::string& text, const SDL_Color& fg)
{
    return sdl::Surface(
        TTF_RenderUTF8_Solid(_font.get(), text.c_str(), fg));
}

sdl::Surface Font::renderUtf8Shaded(
    const std::string& text, const SDL_Color& fg, const SDL_Color& bg)
{
    return sdl::Surface(
        TTF_RenderUTF8_Shaded(_font.get(), text.c_str(), fg, bg));
}

sdl::Surface Font::renderUtf8Blended(const std::string& text, const SDL_Color& fg)
{
    sdl::Surface surface(
        TTF_RenderUTF8_Blended(_font.get(), text.c_str(), fg));

    if (!surface) {
        throw TtfError("TTF_RenderUTF8_Blended");
    }       

    return surface;
}

Font openFont(const std::string& file, int ptsize)
{
    return Font(TTF_OpenFont(file.c_str(), ptsize));
}

} // namespace ttf

namespace sdl {

/**
 * Surface
 */

Surface::Surface()
    : Surface(nullptr)
{ }

Surface::Surface(SDL_Surface* ptr)
    : _surface(ptr)
{ }

Surface::Surface(const Surface& other)
    : _surface(nullptr)
{
    if (other._surface) {
        _surface = SDL_ConvertSurface(
            other._surface, other._surface->format, 0);
    }
}

Surface::~Surface()
{
    if (_surface) {
        SDL_FreeSurface(_surface);
    }
}

Surface& Surface::operator=(const Surface& other)
{
    if (_surface) {
        SDL_FreeSurface(_surface);
    }

    if (other._surface) {
        _surface = SDL_ConvertSurface(
            other._surface, other._surface->format, 0);
    } else {
        _surface = nullptr;
    }

    return *this;        
}

Surface& Surface::operator=(Surface&& other)
{
    std::swap(_surface, other._surface);
    return *this;
}

Surface::operator bool() const
{
    return !!_surface;
}

const SDL_PixelFormat& Surface::format() const
{
    return *_surface->format;
}

int Surface::w() const
{
    return _surface->w;
}

int Surface::h() const
{
    return _surface->h;
}

int Surface::pitch() const
{
    return _surface->pitch;
}

void* Surface::pixels()
{
    return _surface->pixels;
}

void* Surface::userdata()
{
    return _surface->userdata;
}

const SDL_Rect& Surface::clipRect() const
{
    return _surface->clip_rect;
}

int& Surface::refcount()
{
    return _surface->refcount;
}

void Surface::fillRect(const SDL_Rect& rect, Uint32 color)
{
    SDL_FillRect(_surface, &rect, color);
}

/**
 * Texture
 */

Texture::Texture()
    : _texture(nullptr)
{ }

Texture::Texture(SDL_Texture* ptr)
    : _texture(ptr, SDL_DestroyTexture)
{ }

/**
 * Window
 */

Window::Window(
        const std::string& title,
        int x,
        int y,
        int w,
        int h,
        Uint32 flags)
    : _window(
        SDL_CreateWindow(title.c_str(), x, y, w, h, flags),
        SDL_DestroyWindow)
{ }

Window::Size Window::size() const
{
    Size size;
    SDL_GetWindowSize(_window.get(), &size.w, &size.h);
    return size;
}

Renderer Window::createRenderer(int index, Uint32 flags)
{
    return Renderer(_window, index, flags);
}

/**
 * Renderer
 */

Renderer::Renderer(std::shared_ptr<SDL_Window> window, int index, Uint32 flags)
    : _window(window)
    , _renderer(
        SDL_CreateRenderer(_window.get(), index, flags),
        SDL_DestroyRenderer)
{
    SDL_SetRenderDrawBlendMode(_renderer.get(), SDL_BLENDMODE_BLEND);
}

Texture Renderer::createTextureFromSurface(const Surface& surface) const
{
    return Texture(
        SDL_CreateTextureFromSurface(
            _renderer.get(), surface._surface));
}

void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(_renderer.get(), r, g, b, a);
}

void Renderer::clear()
{
    SDL_RenderClear(_renderer.get());
}

void Renderer::present()
{
    SDL_RenderPresent(_renderer.get());
}

void Renderer::fillRect(const SDL_Rect& rect)
{
    SDL_RenderFillRect(_renderer.get(), &rect);
}

void Renderer::copy(
    const Texture& texture,
    const SDL_Rect* srcrect,
    const SDL_Rect* dstrect)
{
    SDL_RenderCopy(_renderer.get(), texture._texture.get(), srcrect, dstrect);
}


} // namespace sdl

namespace img {

sdl::Surface load(const std::string& file)
{
    return sdl::Surface(IMG_Load(file.c_str()));
}

} // namespace img
