#include "sdl_wrapper.hpp"
#include "exceptions.hpp"

SdlError::SdlError(const std::string& sdlFunction)
    : _message("SDL error in " + sdlFunction + ": " + SDL_GetError())
{ }

const char* SdlError::what() const noexcept
{
    return _message.c_str();
}

ImgError::ImgError(const std::string& sdlImageFunction)
    : _message("SDL_image error in " + sdlImageFunction + ": " + IMG_GetError())
{ }

const char* ImgError::what() const noexcept
{
    return _message.c_str();
}

TtfError::TtfError(const std::string& sdlTtfFunction)
    : _message("SDL_ttf error in " + sdlTtfFunction + ": " + TTF_GetError())
{ }

const char* TtfError::what() const noexcept
{
    return _message.c_str();
}
