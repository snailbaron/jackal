#pragma once

#include "view.hpp"
#include "gui_element.hpp"
#include "resources.hpp"
#include "sdl.hpp"

#include <string>
#include <memory>
#include <functional>


class WrittenText {
public:
    WrittenText(
        std::string text,
        r::Font fontId,
        const Color& textColor,
        const ScreenVector& size);

    const std::string& text() const { return _text; }
    r::Font fontId() const { return _fontId; }
    sdl::Texture& texture() { return _texture; }
    const ScreenVector& screenOffset() const { return _screenOffset; }
    const ScreenRect& textureRect() const { return _textureRect; }

    void resize(const ScreenVector& size);

private:
    std::string _text;
    r::Font _fontId;
    Color _textColor;
    sdl::Texture _texture;
    ScreenVector _screenOffset;
    ScreenRect _textureRect;
};

class SolidBackground : public GuiElement {
public:
    SolidBackground(const Color& color);

    bool contains(const ScreenVector& point) override;
    void render(View& window) override;

private:
    Color _color;
};

class Button : public GuiElement {
public:
    Button(
        const RelativeRect& location,
        std::string&& text,
        std::function<void()> action);

    bool contains(const ScreenVector& point) override;
    void hoverOn() override;
    void hoverOff() override;
    void press() override;
    void release() override;
    void render(View& view) override;
    void action() override;
    void resize() override;

private:
    ScreenRect screenLocation() const;
    ScreenVector screenSize() const;
    ScreenRect textArea() const;
    int frameWidth() const;
    int textMargin() const;

    RelativeRect _location;
    WrittenText _text;
    Color _bgColor;
    Color _frameColor;
    std::function<void()> _action;
};