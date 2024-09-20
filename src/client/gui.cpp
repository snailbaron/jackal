#include "gui.hpp"
#include "app.hpp"
#include <algorithm>
#include <cassert>
#include <utility>

namespace {

const Color NORMAL_BG_COLOR{ 255, 230, 160 };
const Color PRESSED_BG_COLOR{ 240, 210, 140 };
const Color PASSIVE_FRAME_COLOR{ 160, 90, 0 };
const Color ACTIVE_FRAME_COLOR{ 255, 160, 30 };
const float RELATIVE_FRAME_WIDTH{ 0.1f };
const float RELATIVE_TEXT_MARGIN{ 0.1f };
const Color TEXT_COLOR{ 0, 0, 0 };
const r::Font FONT{r::Font::FyodorBold };

} // namespace

WrittenText::WrittenText(
        std::string text,
        r::Font fontId,
        const Color& textColor,
        const ScreenVector& size)
    : _text(text)
    , _fontId(fontId)
    , _textColor(textColor)
{
    resize(size);
}

void WrittenText::resize(const ScreenVector& size)
{
    // Create font to fit given text vertically
    ttf::Font font = app().res().font(_fontId, size.y);

    // If it does not fit horizontally, use smaller font size
    ttf::Font::Size renderedSize = font.sizeUtf8(_text);
    if (renderedSize.w > size.x) {
        font = app().res().font(_fontId, size.y * size.x / renderedSize.w);
        renderedSize = font.sizeUtf8(_text);
    }
    ScreenVector renderedSizeVector { renderedSize.w, renderedSize.h };

    // Create texture with text
    sdl::Surface surface = font.renderUtf8Blended(_text, _textColor.sdl());
    _texture = app().window().createTextureFromSurface(surface);

    // Save offsets
    _screenOffset = (size - renderedSizeVector).loClamped(0) / 2;

    ScreenVector textureOffset = (renderedSizeVector - size).loClamped(0) / 2;
    _textureRect = ScreenRect(ScreenVector(0, 0), renderedSizeVector).padded(textureOffset);
}

SolidBackground::SolidBackground(const Color& color)
    : _color(color)
{ }

bool SolidBackground::contains(const ScreenVector&)
{
    return false;
}

void SolidBackground::render(View&)
{
    app().window().clear(_color);
}

Button::Button(
        const RelativeRect& location,
        std::string&& text,
        std::function<void()> action)
    : _location(location)
    , _text(text, FONT, TEXT_COLOR, textArea().size)
    , _bgColor(NORMAL_BG_COLOR)
    , _frameColor(PASSIVE_FRAME_COLOR)
    , _action(action)
{ }

bool Button::contains(const ScreenVector& point)
{
    return screenLocation().contains(point);
}

void Button::hoverOn()
{
    _frameColor = ACTIVE_FRAME_COLOR;
}

void Button::hoverOff()
{
    _frameColor = PASSIVE_FRAME_COLOR;
}

void Button::press()
{
    _bgColor = PRESSED_BG_COLOR;
}

void Button::release()
{
    _bgColor = NORMAL_BG_COLOR;
}

void Button::action()
{
    _action();
}

void Button::render(View&)
{
    Window& window = app().window();
    ScreenRect location = screenLocation();

    window.drawRect(location, _frameColor);
    window.drawRect(location.padded(frameWidth()), _bgColor);
    window.drawTexturePart(_text.texture(), textArea().padded(_text.screenOffset()), _text.textureRect());
}

ScreenRect Button::screenLocation() const
{
    return _location.screen(app().window().size());
}

ScreenVector Button::screenSize() const
{
    return _location.size.screen(app().window().size());
}

ScreenRect Button::textArea() const
{
    return screenLocation().padded(frameWidth() + textMargin());
}

int Button::frameWidth() const
{
    return static_cast<int>(
        std::min(screenSize().x, screenSize().y) * RELATIVE_FRAME_WIDTH);
}

int Button::textMargin() const
{
    return static_cast<int>(
        std::min(screenSize().x, screenSize().y) * RELATIVE_TEXT_MARGIN);
}

void Button::resize()
{
    _text.resize(textArea().size);
}