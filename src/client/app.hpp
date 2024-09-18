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

class App : public Singleton<App> {
public:
    App(const std::filesystem::path& configFile);

    void run();

    const Window& window() const { return _window; }
    Window& window() { return _window; }

    Resources& res() { return _resources; }
    const ClientGame& game() const { return _game; }
    ClientGame& game() { return _game; }

private:
    void setMainMenu();
    void setOptionsMenu();
    void setGameField();

    Config _config;
    Window _window;
    Resources _resources;

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
