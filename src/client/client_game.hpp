#pragma once

#include "../core/JackalGame.h"
#include "../core/GameState.h"

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

    ClientGame(std::shared_ptr<JackalGame> game);

    const GameState& coreState() const;
    int activePlayer() const;
    const ::Cell& cell(int x, int y) const;
    TypeCellEnum cellType(int x, int y) const;
    std::set<Cell> validMoves() const;

    void startNewGame(int playerCount);
    void activateCell(const Cell& cell);
    void update();

private:
    enum class State {
        SelectSubject,
        SelectTarget,
    };

    std::shared_ptr<JackalGame> _game;
    GameState _coreState;

    State _state;

    int _movingPirate = 0;
    std::map<Cell, int> _validMoves;
    std::vector<Position> _moveCandidates;

    int _playerCount;
    int _activePlayer;
};

ClientGame::Cell cellFromPoint(const Point& point);
ClientGame::Cell pirateCell(const Pirate& pirate);

std::ostream& operator<<(std::ostream& os, const ClientGame::Cell& cell);
std::ostream& operator<<(std::ostream& os, const ClientGame::Position& position);
