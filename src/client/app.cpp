#include "app.hpp"
#include "gui.hpp"
#include "client_game.hpp"
#include "sdl_wrapper.hpp"
#include <json.hpp>
#include <stdexcept>
#include <utility>
#include <fstream>

using Json = nlohmann::json;

App::App(const std::string& configFile)
    : _game(std::make_shared<JackalGame>())
{
    initMediaLayer();

    _config = readAppConfig(configFile);
    _window = std::make_shared<Window>(_config.window);
    _resources = std::make_unique<Resources>(_window);
     
    _view = std::make_unique<View>(_window);
}

App::~App()
{
    _resources = nullptr;
    _window = nullptr;
    killMediaLayer();
}

void App::run()
{
    setMainMenu();

    bool done = false;
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

        _game.update();
        _view->render();
    }

}

void App::initMediaLayer()
{
    // Check if any of SDL subsystems is already initialized
    if (SDL_WasInit(0)) {
        throw std::runtime_error("ViewSystem was already init");
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("ViewSystem: failed to initialize SDL");
    }

    // Initialize SDL_image
    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
        throw std::runtime_error("ViewSystem: failed to initialize SDL_image");
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        throw std::runtime_error("ViewSystem: failed to initialize SDL_ttf");
    }
}

void App::killMediaLayer()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

AppConfiguration App::readAppConfig(const std::string& configFile)
{
    std::ifstream configStream(configFile);
    if (!configStream.is_open()) {
        throw std::runtime_error(
            "Failed to read app configuration file: " + configFile);
    }

    Json json;
    configStream >> json;

    // TODO: Do the stuff below in a try-catch block, to fall back to default
    // configuration if the one being read is corrupted. Maybe rewrite the file
    // with default data, in whole or corrupted parts only.

    AppConfiguration config;
    config.window.title = json["window"]["title"].get<std::string>();
    config.window.width = json["window"]["width"];
    config.window.height = json["window"]["height"];
    config.window.fullscreen = json["window"]["fullscreen"];
    return config;
}

void App::setMainMenu()
{
    _view->clear();
    _view->createGuiElement<SolidBackground>(Color{ 255, 250, 240 });

    _view->createGuiElement<Button>(
        RelativeRect(0.3, 0.2, 0.4, 0.1),
        "New game",
        [this] {
            _game.startNewGame(_config.numberOfPlayers);
            setGameField();
        });

    _view->createGuiElement<Button>(
        RelativeRect(0.3, 0.4, 0.4, 0.1),
        "Options",
        [this] {
            setOptionsMenu();
        });

    _view->createGuiElement<Button>(
        RelativeRect(0.3, 0.6, 0.4, 0.1),
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

    _view->createGuiElement<SolidBackground>(Color{ 255, 250, 240 });
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

    _view->createGuiElement<SolidBackground>(Color{ 255, 250, 240 });
    _view->addGuiElement(_view->createScene());
    _view->createGuiElement<Button>(
        RelativeRect(0.9, 0.05, 0.08, 0.05),
        "Menu",
        [this] {
            setMainMenu();
        });
}
