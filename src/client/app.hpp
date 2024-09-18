#pragma once

#include "singleton.hpp"
#include "window.hpp"
#include "resources.hpp"
#include "view.hpp"
#include "client_game.hpp"
#include "sdl.hpp"
#include "../core/JackalGame.h"

struct AppConfiguration {
    WindowConfiguration window;
    int numberOfPlayers = 4;
};

/**
 * Class representing the whole application.
 */
class App : public Singleton<App> {
public:
    App(const std::string& configFile);
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
    AppConfiguration readAppConfig(const std::string& configFile);

    void setMainMenu();
    void setOptionsMenu();
    void setGameField();

    AppConfiguration _config;
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
