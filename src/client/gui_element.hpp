#pragma once

#include "geometry.hpp"

class View;

class GuiElement {
public:
    virtual ~GuiElement() = default;

    virtual bool contains(const ScreenVector&) { return false; }
    virtual void render(View& view) = 0;

    virtual void hoverOn() {}
    virtual void hoverOff() {}
    virtual void pointer(const ScreenVector&) {}
    virtual void press() {}
    virtual void release() {}
    virtual void action() {}
    virtual void grabOn() {}
    virtual void grabOff() {}
    virtual void drag(const ScreenVector&) {}
    virtual void resize() {}
};
