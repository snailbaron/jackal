#include "app.hpp"

#include "client_game.hpp"
#include "gui.hpp"
#include "sdl.hpp"
#include "timer.hpp"

#include <stdexcept>
#include <utility>
#include <fstream>

App::App(const std::filesystem::path& configFile)
{
    _config = loadConfig(configFile);
    _window = Window{_config.window};
    _resources = Resources{x::exeDir() / "jackal.data", _window.renderer()};

    _view = std::make_unique<View>(_window);
}

void App::run()
{
    setMainMenu();

    bool done = false;
    auto timer = FrameTimer{_config.fps};
    while (!done) {
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            switch (evt.type) {
            case SDL_QUIT:
                done = true;
                break;

            default:
                _view->processEvent(evt);
                break;
            }
        }

        if (timer() > 0) {
            _game.update();
            _view->render();
        }

        timer.relax();
    }

}

void App::setMainMenu()
{
    _view->clear();
    _view->createGuiElement<SolidBackground>(SDL_Color{255, 250, 240});

    _view->createGuiElement<Button>(
        RelativeRect(0.3, 0.3, 0.4, 0.07),
        "Hot Seat",
        [this] {
            _game.startLocalGame(_config.numberOfPlayers);
            setGameField();
        });

    _view->createGuiElement<Button>(
        RelativeRect(0.3, 0.4, 0.4, 0.07),
        "New Remote Game",
        [this] {
            _game.startRemoteGame();
            setGameField();
        });

    _view->createGuiElement<Button>(
        RelativeRect(0.3, 0.5, 0.4, 0.07),
        "Options",
        [this] {
            setOptionsMenu();
        });

    _view->createGuiElement<Button>(
        RelativeRect(0.3, 0.7, 0.4, 0.07),
        "Quit",
        [] {
            SDL_Event evt;
            evt.type = SDL_QUIT;
            SDL_PushEvent(&evt);
        });
}

void App::setOptionsMenu()
{
    _view->clear();

    _view->createGuiElement<SolidBackground>(SDL_Color{255, 250, 240});
    _view->createGuiElement<Button>(
        RelativeRect(0.2, 0.4, 0.6, 0.06),
        "[PLACEHOLDER. OPTIONS MENU IS NOT IMPLEMENTED.]",
        [] {});
    _view->createGuiElement<Button>(
        RelativeRect(0.4, 0.8, 0.2, 0.1),
        "Back",
        [this] {
            setMainMenu();
        });
}

void App::setGameField()
{
    _view->clear();

    _view->createGuiElement<SolidBackground>(SDL_Color{255, 250, 240});
    _view->createGuiElement<Field>();
    _view->createGuiElement<Button>(
        RelativeRect(0.9, 0.05, 0.08, 0.05),
        "Menu",
        [this] {
            setMainMenu();
        });
    _view->createGuiElement<MoveIndicator>(
        RelativeRect(0.9, 0.15, 0.05, 0.05));
}
