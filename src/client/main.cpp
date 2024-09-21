#include "app.hpp"

#include <x.hpp>

#include <cstdlib>
#include <exception>
#include <iostream>

int main(int, char*[])
{
    auto sdlInit = sdl::Init{SDL_INIT_AUDIO | SDL_INIT_VIDEO};
    auto imgInit = img::Init{IMG_INIT_PNG};
    auto ttfInit = ttf::Init{};

    App::initialize(x::exeDir() / "jackal.yaml");
    app().run();

    return 0;
}