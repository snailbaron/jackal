#pragma once

#include "../core/JackalGame.h"
#include "../core/GameState.h"
#include <memory>
#include <map>
#include <set>
#include <ostream>

class ClientGame {
public:
    static const int FieldSize;

    struct Cell {
        int x;
        int y;
    };

    struct Position {
        Cell cell;
        int depth;
    };

    ClientGame(std::shared_ptr<JackalGame> game);
    
    const GameState& coreState() const;
    int activePlayer() const;
    TypeCellEnum cellType(int x, int y) const;
    std::set<Cell> validMoves() const;

    void startNewGame(int playerCount);
    void activateCell(const Cell& cell);
    void update();

private:
    enum class State;

    std::shared_ptr<JackalGame> _game;
    GameState _coreState;

    State _state;

    int _movingPirate;
    std::map<Cell, int> _validMoves;
    std::vector<Position> _moveCandidates;

    int _playerCount;
    int _activePlayer;
};

ClientGame::Cell pirateCell(const Pirate& pirate);

bool operator==(const ClientGame::Cell& left, const ClientGame::Cell& right);
bool operator!=(const ClientGame::Cell& left, const ClientGame::Cell& right);
bool operator<(const ClientGame::Cell& left, const ClientGame::Cell& right);
bool operator>(const ClientGame::Cell& left, const ClientGame::Cell& right);
bool operator<=(const ClientGame::Cell& left, const ClientGame::Cell& right);
bool operator>=(const ClientGame::Cell& left, const ClientGame::Cell& right);

std::ostream& operator<<(std::ostream& os, const ClientGame::Cell& cell);

bool operator==(const ClientGame::Position& left, const ClientGame::Position& right);
bool operator!=(const ClientGame::Position& left, const ClientGame::Position& right);
bool operator<(const ClientGame::Position& left, const ClientGame::Position& right);
bool operator>(const ClientGame::Position& left, const ClientGame::Position& right);
bool operator<=(const ClientGame::Position& left, const ClientGame::Position& right);
bool operator>=(const ClientGame::Position& left, const ClientGame::Position& right);

std::ostream& operator<<(std::ostream& os, const ClientGame::Position& position);
