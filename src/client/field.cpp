#include "field.hpp"
#include "view.hpp"
#include "app.hpp"
#include "log.hpp"
#include "../core/Cell.h"
#include "../core/GameState.h"
#include <map>
#include <cmath>
#include <tuple>
#include <ostream>
#include <iostream>

namespace {

// Magic to read GameState correctly
const int FIELD_DATA_SIZE = ClientGame::FieldSize;
const int FIELD_DATA_OFFSET = 2;

// Field geometry
const int GRID_WIDTH = 3;
const int CELL_SIZE = 64;

// Field colors
const Color GRID_COLOR { 100, 100, 100 };
const Color CELL_OUTLINE_COLOR { 255, 150, 0 };

// Calculated field parameters
const int CELL_DELTA = GRID_WIDTH + CELL_SIZE;
const int FIELD_SIZE = CELL_DELTA * FIELD_DATA_SIZE + GRID_WIDTH;


const std::map<TypeCellEnum, TextureId> cellTypeTextureIds {
    { Horse, TextureId::Horse },
    { Ice, TextureId::Ice },
    { Cannibal, TextureId::Cannibal },
    { Fort, TextureId::Fort },
    { NativeWoman, TextureId::NativeWoman },
    { Pitfall, TextureId::Pitfall },
    { Rum, TextureId::Rum },
    { Crocodile, TextureId::Crocodile },
    { DifficultTerrain, TextureId::DifficultTerrain },
    { Treasure, TextureId::Treasure },
    { CannonUp, TextureId::CannonUp },
    { CannonLeft, TextureId::CannonLeft },
    { CannonRight, TextureId::CannonRight },
    { CannonDown, TextureId::CannonDown },
    { Baloon, TextureId::Baloon },
    { Airplane, TextureId::Airplane },
    { Grass, TextureId::Grass },
    { Closed, TextureId::Closed },
    { Water, TextureId::Water },
    { Root, TextureId::Root },
};

sdl::Texture textureForCellType(TypeCellEnum cellType)
{
    if (cellType >= 256) {
        return app().res().texture(cellTypeTextureIds.at(cellType));
    }
    else {
        ArrowDescription arrows{};
        for (int i = 0; i < 8; i++) {
            arrows[i] = (cellType & (1 << i)) > 0;
        }
        return app().res().arrowTexture(arrows);
    }
}

} // namespace

Field::Field()
    : _position(20, 20)
{ }

void Field::render(View& view)
{
    // Draw grid
    app().window()->drawGrid(
        _position.x, _position.y,
        FIELD_DATA_SIZE, FIELD_DATA_SIZE,
        CELL_SIZE, CELL_SIZE,
        GRID_WIDTH,
        GRID_COLOR);

    // Mark the cell pointed to
    if (_focused) {
        app().window()->drawRect(
            cellOuterRect(_activeCell), CELL_OUTLINE_COLOR);
    }

    // Draw cells
    for (int x = 0; x < FIELD_DATA_SIZE; x++) {
        for (int y = 0; y < FIELD_DATA_SIZE; y++) {
            TypeCellEnum cellType = app().game().cellType(x, y);
			sdl::Texture texture = textureForCellType(cellType);
            app().window()->drawTexture(texture, cellInnerRect({x, y}));
        }
    }

    // Draw pirates
    std::map<int, std::map<ClientGame::Cell, int>> pirateCount;
    for (const Pirate& pirate : app().game().coreState().pirates) {
        pirateCount[pirate.id_player][pirateCell(pirate)]++;
    }

    for (const auto& i : pirateCount) {
        int playerId = i.first;
		sdl::Texture pirateTexture;
		switch (playerId) {
		case 0: 
			pirateTexture = app().res().texture(TextureId::PirateRed);
			break;
		case 1:
			pirateTexture = app().res().texture(TextureId::PirateGreen);
			break;
		case 2:
			pirateTexture = app().res().texture(TextureId::PirateBlue);
			break;
		case 3:
			pirateTexture = app().res().texture(TextureId::PirateYellow);
			break;
		}

		for (const auto& j : i.second) {
            const ClientGame::Cell& pirateCell = j.first;
            int pirateCount = j.second;

            auto rects = pirateRects(pirateCell, pirateCount);
            for (const auto& rect : rects) {
                app().window()->drawTexture(pirateTexture, rect);
            }
        }
    }

    // Draw valid moves
    for (const auto& cell : app().game().validMoves()) {
        ScreenRect rect = cellInnerRect(cell);
        app().window()->drawRect(rect, { 255, 0, 0, 150 });
    }
}

bool Field::contains(const ScreenVector& point)
{
    ScreenRect fieldRect(_position, {FIELD_SIZE, FIELD_SIZE});
    return fieldRect.contains(point);
}

void Field::drag(const ScreenVector& delta)
{
    _position += delta;
}

void Field::grabOff()
{
    // If game field is dragged too far outside the screen, return it back.
    int xMiddle = app().window()->width() / 2;
    int yMiddle = app().window()->height() / 2;

    if (_position.x > xMiddle) {
        _position.x = xMiddle;
    } else if (_position.x + FIELD_SIZE < xMiddle) {
        _position.x = xMiddle - FIELD_SIZE;
    }
    if (_position.y > yMiddle) {
        _position.y = yMiddle;
    } else if (_position.y + FIELD_SIZE < yMiddle) {
        _position.y = yMiddle - FIELD_SIZE;
    }
}

void Field::hoverOn()
{
    _focused = true;
}

void Field::hoverOff()
{
    _focused = false;
}

void Field::pointer(const ScreenVector& point)
{
    _activeCell = cellUnderPoint(point);
}

void Field::press()
{
    _pressedCell = _activeCell;
}

void Field::release()
{
    if (_pressedCell != _activeCell) {
        return;
    }
    app().game().activateCell(_pressedCell);
}

ScreenRect Field::cellOuterRect(const ClientGame::Cell& cell)
{
    return ScreenRect(
        _position.x + cell.y * CELL_DELTA,
        _position.y + (ClientGame::FieldSize - 1 - cell.x) * CELL_DELTA,
        CELL_DELTA + GRID_WIDTH,
        CELL_DELTA + GRID_WIDTH);
}

ScreenRect Field::cellInnerRect(const ClientGame::Cell& cell)
{
    return cellOuterRect(cell).padded(GRID_WIDTH);
}

ClientGame::Cell Field::cellUnderPoint(const ScreenVector& point) const
{
    ScreenVector lowerLeftCorner { _position.x, _position.y + FIELD_SIZE };
    ScreenVector lowerLeftGridMiddle = lowerLeftCorner +
        ScreenVector{GRID_WIDTH, -GRID_WIDTH};
    ScreenVector relativePoint = point - lowerLeftGridMiddle;

    return {
        std::min(-relativePoint.y / CELL_DELTA, FIELD_DATA_SIZE - 1),
        std::min(relativePoint.x / CELL_DELTA, FIELD_DATA_SIZE - 1)
    };
}

std::vector<ScreenRect> Field::pirateRects(const ClientGame::Cell& cell, int pirateCount)
{
    float x = 0.f, y = -1.f;
    float angle = static_cast<float>(2 * M_PI / pirateCount);
    float co = std::cos(angle);
    float si = std::sin(angle);

    ScreenRect baseRect = cellInnerRect(cell).zoomed(0.5f);
    std::vector<ScreenRect> result;
    for (int i = 0; i < pirateCount; i++) {
        ScreenVector offset {
            static_cast<int>(x * baseRect.size.x / 2),
            static_cast<int>(y * baseRect.size.y / 2)};
        result.push_back(baseRect.offset(offset));

        float xx = co * x + si * y;
        float yy = -si * x + co * y;
        x = xx;
        y = yy;
    }

    return result;
}
