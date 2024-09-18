#include "app.hpp"
#include "fs.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

int main(int, char*[]) try
{
    auto sdlInit = sdl::Init{SDL_INIT_AUDIO | SDL_INIT_VIDEO};
    auto imgInit = img::Init{IMG_INIT_PNG};
    auto ttfInit = ttf::Init{};

    App::initialize(fs::exeDir() / "jackal.yaml");
    app().run();

    return 0;
} catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
}