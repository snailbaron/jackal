#include "geometry.hpp"
#include <algorithm>

SDL_Rect ScreenRect::sdl() const
{
    return SDL_Rect { origin.x, origin.y, size.x, size.y };
}

ScreenVector RelativeVector::screen(const ScreenVector& base) const
{
    return ScreenVector {
        static_cast<int>(std::round(base.x * x)),
        static_cast<int>(std::round(base.y * y))
    };
}

ScreenRect RelativeRect::screen(const ScreenVector& base) const
{
    return ScreenRect(origin.screen(base), size.screen(base));
}
