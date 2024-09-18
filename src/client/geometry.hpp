#pragma once

#include "basic_geometry.hpp"
#include "sdl.hpp"

struct ScreenTag {};

struct ScreenVector : public Vector<int, ScreenTag> {
    using V = Vector<int, ScreenTag>;
    using V::Vector;

    ScreenVector() : V() {}
    ScreenVector(const V& base) : V(base) {}
};

struct ScreenRect : public Rect<ScreenVector, ScreenTag> {
    using R = Rect<ScreenVector, ScreenTag>;
    using R::Rect;

    ScreenRect() : R() {}
    ScreenRect(const R& base) : R(base) {}

    SDL_Rect sdl() const;
};

struct RelativeTag {};

struct RelativeVector : public Vector<double, RelativeTag> {
    using V = Vector<double, RelativeTag>;
    using V::Vector;

    RelativeVector() : V() {}
    RelativeVector(const V& base) : V(base) {}

    ScreenVector screen(const ScreenVector& base) const;
};

struct RelativeRect : public Rect<RelativeVector, RelativeTag> {
    using R = Rect<RelativeVector, RelativeTag>;
    using R::Rect;

    RelativeRect() : R() {}
    RelativeRect(const R& base) : R(base) {}

    ScreenRect screen(const ScreenVector& base) const;
};
