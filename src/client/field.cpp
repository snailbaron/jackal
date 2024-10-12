#include "field.hpp"

#include "app.hpp"
#include "gui.hpp"
#include "log.hpp"
#include "view.hpp"

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

// Field geometry
const int GRID_WIDTH = 3;
const int CELL_SIZE = 64;

// Field colors
const SDL_Color GRID_COLOR { 100, 100, 100 };
const SDL_Color CELL_OUTLINE_COLOR { 255, 150, 0 };

// Calculated field parameters
const int CELL_DELTA = GRID_WIDTH + CELL_SIZE;
const int FIELD_SIZE = CELL_DELTA * FIELD_DATA_SIZE + GRID_WIDTH;

const std::map<TypeCellEnum, r::Sprite> cellTypeTextureIds {
    { Horse, r::Sprite::Horse },
    { Ice, r::Sprite::Ice },
    { Cannibal, r::Sprite::Cannibal },
    { Fort, r::Sprite::Fort },
    { NativeWoman, r::Sprite::NativeWoman },
    { Pitfall, r::Sprite::Pitfall },
    { Rum, r::Sprite::Rum },
    { Crocodile, r::Sprite::Crocodile },
    { DifficultTerrain, r::Sprite::DifficultTerrain },
    { Treasure, r::Sprite::Treasure },
    { CannonUp, r::Sprite::CannonUp },
    { CannonLeft, r::Sprite::CannonLeft },
    { CannonRight, r::Sprite::CannonRight },
    { CannonDown, r::Sprite::CannonDown },
    { Baloon, r::Sprite::Baloon },
    { Airplane, r::Sprite::Airplane },
    { Grass, r::Sprite::Grass },
    { Closed, r::Sprite::Closed },
    { Water, r::Sprite::Water },
    { Root, r::Sprite::Root },
};

r::Sprite pirateTexture(const Pirate& pirate)
{
    auto mapping = std::map<std::pair<int, MovieType>, r::Sprite>{
        {{0, MovieType::none}, r::Sprite::PirateRed},
        {{0, MovieType::drink_rum}, r::Sprite::PirateRedRum},
        {{0, MovieType::run}, r::Sprite::PirateRedRun},
        {{1, MovieType::none}, r::Sprite::PirateGreen},
        {{1, MovieType::drink_rum}, r::Sprite::PirateGreenRum},
        {{1, MovieType::run}, r::Sprite::PirateGreenRun},
        {{2, MovieType::none}, r::Sprite::PirateBlue},
        {{2, MovieType::drink_rum}, r::Sprite::PirateBlueRum},
        {{2, MovieType::run}, r::Sprite::PirateBlueRun},
        {{3, MovieType::none}, r::Sprite::PirateYellow},
        {{3, MovieType::drink_rum}, r::Sprite::PirateYellowRum},
        {{3, MovieType::run}, r::Sprite::PirateYellowRun},
    };
    return mapping.at({pirate.id_player, pirate.movie});
}

sdl::Texture& textureForCellType(TypeCellEnum cellType)
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

void Field::render(View&)
{
    // Draw grid
    app().window().drawGrid(
        _position.x, _position.y,
        FIELD_DATA_SIZE, FIELD_DATA_SIZE,
        CELL_SIZE, CELL_SIZE,
        GRID_WIDTH,
        GRID_COLOR);

    // Mark the cell pointed to
    if (_focused) {
        app().window().drawRect(
            cellOuterRect(_activeCell), CELL_OUTLINE_COLOR);
    }

    // Draw cells
    for (int x = 0; x < FIELD_DATA_SIZE; x++) {
        for (int y = 0; y < FIELD_DATA_SIZE; y++) {
            const auto& cell = app().game().cell(x, y);
			sdl::Texture& texture = textureForCellType((TypeCellEnum)cell.type.type);
            auto rect = cellInnerRect({x, y});
            app().window().drawTexture(texture, rect);

            auto coinRect = ScreenRect{
                rect.origin + rect.size * 3 / 4,
                rect.size / 4
            };
            for (int i = 0; i < cell.money; i++) {
                auto& coinTexture = app().res().texture(r::Sprite::Coin);
                app().window().drawTexture(coinTexture, coinRect);
                coinRect.origin += {0, -5};
            }

            size_t cellDepth = 1;
            for (auto* p = &cell; p->next; p = p->next) {
                cellDepth++;
            }
            if (cellDepth > 1) {
                auto depthText = WrittenText{std::to_string(cellDepth), r::Font::FyodorBold, SDL_Color{200, 0, 200}, {16, 16}};
                auto depthTextRect = ScreenRect{
                    rect.origin + ScreenVector{0, rect.size.y * 3 / 4},
                    rect.size / 4
                };
                app().window().drawTexturePart(
                    depthText.texture(), depthTextRect.padded(depthText.screenOffset()), depthText.textureRect());
            }
        }
    }

    // draw ships
    for (const auto& ship : app().game().coreState().ships) {
        auto& shipTexture = app().res().texture(r::Sprite::Ship);
        app().window().drawTexture(shipTexture, cellInnerRect(cellFromPoint(ship)));
    }

    // Draw pirates
    std::map<ClientGame::Cell, std::vector<const Pirate*>> piratesByCell;
    for (const Pirate& pirate : app().game().coreState().pirates) {
        piratesByCell[pirateCell(pirate)].push_back(&pirate);
    }

    for (const auto& [cell, pirates] : piratesByCell) {
        auto rects = pirateRects(cell, pirates.size());
        for (size_t i = 0; i < pirates.size(); i++) {
            const auto& pirate = *pirates.at(i);
            const auto& rect = rects.at(i);

            auto& texture = app().res().texture(pirateTexture(pirate));
            app().window().drawTexture(texture, rect);

            if (pirate.money) {
                auto& coinTexture = app().res().texture(r::Sprite::Coin);
                auto coinRect = ScreenRect{
                    rect.origin + rect.size / 2,
                    rect.size / 2
                };
                app().window().drawTexture(coinTexture, coinRect);
            }

            if (pirate.place.z > 1) {
                auto textArea = ScreenRect{
                    rect.origin + ScreenVector{rect.size.x / 2, 0},
                    rect.size / 2
                };
                auto levelText = WrittenText{std::to_string(pirate.place.z), r::Font::FyodorBold, SDL_Color{200, 0, 200}, ScreenVector{16, 16}};
                app().window().drawTexturePart(
                    levelText.texture(), textArea.padded(levelText.screenOffset()), levelText.textureRect());
            }
        }
    }

    // Draw valid moves
    for (const auto& cell : app().game().validMoves()) {
        ScreenRect rect = cellInnerRect(cell);
        app().window().drawRect(rect, { 255, 0, 0, 150 });
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
    int xMiddle = app().window().width() / 2;
    int yMiddle = app().window().height() / 2;

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

std::vector<ScreenRect> Field::pirateRects(const ClientGame::Cell& cell, size_t pirateCount)
{
    float x = 0.f, y = -1.f;
    float angle = static_cast<float>(2 * M_PI / (double)pirateCount);
    float co = std::cos(angle);
    float si = std::sin(angle);

    ScreenRect baseRect = cellInnerRect(cell).zoomed(0.5f);
    std::vector<ScreenRect> result;
    for (size_t i = 0; i < pirateCount; i++) {
        ScreenVector offset {
            static_cast<int>(x * (float)baseRect.size.x / 2),
            static_cast<int>(y * (float)baseRect.size.y / 2)};
        result.push_back(baseRect.offset(offset));

        float xx = co * x + si * y;
        float yy = -si * x + co * y;
        x = xx;
        y = yy;
    }

    return result;
}
