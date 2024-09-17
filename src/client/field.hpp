#pragma once

#include "window.hpp"
#include "gui_element.hpp"
#include "client_game.hpp"
#include "../core/Cell.h"
#include "../core/GameState.h"
#include <vector>
#include <map>

class Field : public GuiElement {
public:
    Field();

    bool contains(const ScreenVector& point) override;
    void render(View&) override;

    void drag(const ScreenVector& delta) override;
    void grabOff() override;
    void hoverOn() override;
    void hoverOff() override;
    void pointer(const ScreenVector& point) override;
    void press() override;
    void release() override;

private:
    ScreenRect cellOuterRect(const ClientGame::Cell& cell);
    ScreenRect cellInnerRect(const ClientGame::Cell& cell);
    ClientGame::Cell cellUnderPoint(const ScreenVector& point) const;
    std::vector<ScreenRect> pirateRects(const ClientGame::Cell& cell, int pirateCount);
    void activateCell(const Cell& cell);

    ScreenVector _position;
    bool _focused = false;
    ClientGame::Cell _activeCell;
    ClientGame::Cell _pressedCell;
};
