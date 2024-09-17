#pragma once

#include "../core/Cell.h"
#include "../core/GameState.h"
#include "window.hpp"
#include "gui_element.hpp"
#include "field.hpp"
#include "sdl_wrapper.hpp"
#include <json.hpp>
#include <map>
#include <memory>
#include <string>
#include <utility>

using nlohmann::json;

class View {
    friend class App;

public:
    View(std::shared_ptr<Window> window);

    std::shared_ptr<TTF_Font> font() { return _font; }

    void addGuiElement(std::shared_ptr<GuiElement> guiElement);

    template <class T, class... Args>
    void createGuiElement(Args&&... args)
    {
        _guiElements.push_back(std::shared_ptr<GuiElement>(
            new T(std::forward<Args>(args)...)));
    }

    std::shared_ptr<GuiElement> createScene();

    void clear();

    void render();

    enum class PointerAction {
        Primary,
        Secondary,
    };
    void processEvent(const SDL_Event& evt);
    void processPointerInput(int x, int y, PointerAction action);

private:
    std::shared_ptr<GuiElement> guiUnderPoint(int x, int y);

    std::shared_ptr<Window> _window;
    std::shared_ptr<TTF_Font> _font;

    std::vector<std::shared_ptr<GuiElement>> _guiElements;
    std::shared_ptr<GuiElement> _focusedGuiElement = nullptr;
    std::shared_ptr<GuiElement> _pressedGuiElement = nullptr;
    std::shared_ptr<GuiElement> _grabbedGuiElement = nullptr;

    std::unique_ptr<Field> _scene;
};