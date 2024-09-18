#pragma once

#include "client_game.hpp"
#include "config.hpp"
#include "resources.hpp"
#include "sdl.hpp"
#include "singleton.hpp"
#include "view.hpp"
#include "window.hpp"

#include "../core/JackalGame.h"

#include <filesystem>

/**
 * Class representing the whole application.
 */
class App : public Singleton<App> {
public:
    App(const std::filesystem::path& configFile);
    ~App();

    void run();

    const Window& window() const { return _window; }
    Window& window() { return _window; }

    Resources& res() { return *_resources; }
    const ClientGame& game() const { return _game; }
    ClientGame& game() { return _game; }

private:
    void initMediaLayer();
    void killMediaLayer();

    void setMainMenu();
    void setOptionsMenu();
    void setGameField();

    Config _config;
    Window _window;
    std::unique_ptr<Resources> _resources;

    ClientGame _game;
    std::unique_ptr<View> _view;
};

/**
 * Jackal System
 */
inline App& app()
{
    return App::instance();
}
