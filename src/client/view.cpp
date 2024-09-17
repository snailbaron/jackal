#include "view.hpp"

View::View(std::shared_ptr<Window> window)
    : _window(window)
{ }

void View::addGuiElement(std::shared_ptr<GuiElement> guiElement)
{
    _guiElements.push_back(guiElement);
}

std::shared_ptr<GuiElement> View::createScene()
{
    return std::shared_ptr<GuiElement>(
        new Field());
}

void View::clear()
{
    _guiElements.clear();
}

void View::render()
{
    _window->clear(Color{255, 0, 255});

    for (auto& gui : _guiElements) {
        gui->render(*this);
    }
    if (_scene) {
        _scene->render(*this);
    }

    _window->present();
}

void View::processEvent(const SDL_Event& evt)
{
    if (evt.type == SDL_MOUSEMOTION) {
        // Find current GUI element under cursor
        auto previous = _focusedGuiElement;
        _focusedGuiElement = guiUnderPoint(evt.motion.x, evt.motion.y);
        if (previous != _focusedGuiElement) {
            // Update hovering
            if (previous) {
                previous->hoverOff();
            }
            if (_focusedGuiElement) {
                _focusedGuiElement->hoverOn();
            }

            // Process mouse movement to/from a button that has been pressed
            if (_pressedGuiElement) {
                if (_pressedGuiElement == previous) {
                    _pressedGuiElement->release();
                } else if (_pressedGuiElement == _focusedGuiElement) {
                    _pressedGuiElement->press();
                }
            }
        }

        // Signal mouse movement to the active element
        if (_focusedGuiElement) {
            _focusedGuiElement->pointer({evt.motion.x, evt.motion.y});
        }

        // Process drag on grabbed element
        if (_grabbedGuiElement) {
            _grabbedGuiElement->drag({evt.motion.xrel, evt.motion.yrel});
        }
    } else if (evt.type == SDL_MOUSEBUTTONDOWN) {
        if (evt.button.button == SDL_BUTTON_LEFT) {
            // GUI element was pressed. Signal this to the element, but do not
            // initiate action until button release.
            if (_focusedGuiElement) {
                _focusedGuiElement->press();
                _pressedGuiElement = _focusedGuiElement;
            }
        } else if (evt.button.button == SDL_BUTTON_RIGHT) {
            // Start dragging. Remember the grabbed element for further
            // signaling of drags to it.
            if (_focusedGuiElement) {
                _grabbedGuiElement = _focusedGuiElement;
                _grabbedGuiElement->grabOn();
            }
        }
    } else if (evt.type == SDL_MOUSEBUTTONUP) {
        if (evt.button.button == SDL_BUTTON_LEFT) {
            // Button was released. If we are inside the element that was
            // initially pressed, signal release, and initiate action.
            if (_pressedGuiElement) {
                if (_pressedGuiElement == _focusedGuiElement) {
                    _pressedGuiElement->release();
                    _pressedGuiElement->action();
                }
                _pressedGuiElement = nullptr;
            }
        } else if (evt.button.button == SDL_BUTTON_RIGHT) {
            // "Drop" the grabbed element, if any.
            if (_grabbedGuiElement) {
                _grabbedGuiElement->grabOff();
                _grabbedGuiElement = nullptr;
            }
        }
    } else if (evt.type == SDL_WINDOWEVENT) {
        if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
            for (auto& gui : _guiElements) {
                gui->resize();
            }
        }
    }
}

void View::processPointerInput(int x, int y, PointerAction)
{
    std::cerr << "Action at " << x << ", " << y << std::endl;
}

std::shared_ptr<GuiElement> View::guiUnderPoint(int x, int y)
{
    // GUI elements are rendered in straight order, so the ones at the end are
    // on top of those in the beginning. To focus the topmost elements first,
    // traverse the list in reversed order.
    for (auto it = _guiElements.rbegin(); it != _guiElements.rend(); it++) {
        if ((*it)->contains({x, y})) {
            return (*it);
        }
    }
    return nullptr;
}