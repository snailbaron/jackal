#pragma once

#include "universal_game.hpp"

#include <GameState.h>

#include <memory>
#include <map>
#include <set>
#include <ostream>

class ClientGame {
public:
    static constexpr int FieldSize = 13;

    struct Cell {
        int x = 0;
        int y = 0;

        constexpr auto operator<=>(const Cell&) const = default;
    };

    struct Position {
        Cell cell;
        int depth;

        constexpr auto operator<=>(const Position&) const = default;
    };

    const GameState& coreState() const;
    int activePlayer() const;
    const ::Cell& cell(int x, int y) const;
    TypeCellEnum cellType(int x, int y) const;
    std::set<Cell> validMoves() const;

    void startLocalGame(int playerCount);
    void startRemoteGame();
    void activateCell(const Cell& cell);
    void update();

private:
    enum class State {
        SelectSubject,
        SelectTarget,
    };

    std::unique_ptr<UniversalGame> _game;
    GameState _coreState;
    State _state;

    int _movingPirate = 0;
    std::map<Cell, int> _validMoves;
    std::vector<Position> _moveCandidates;

    int _activePlayer = 0;
};

ClientGame::Cell cellFromPoint(const Point& point);
ClientGame::Cell pirateCell(const Pirate& pirate);

std::ostream& operator<<(std::ostream& os, const ClientGame::Cell& cell);
std::ostream& operator<<(std::ostream& os, const ClientGame::Position& position);
