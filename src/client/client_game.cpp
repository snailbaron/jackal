#include "client_game.hpp"

#include <algorithm>
#include <cassert>

namespace {

constexpr int PlayerCount = 4;

Point makePoint(const ClientGame::Cell& cell, int depth)
{
    return {cell.x + 2, cell.y + 2, depth};
}

} // namespace

std::ostream& operator<<(std::ostream& os, const ClientGame::Cell& cell)
{
    os << "(" << cell.x << ", " << cell.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const ClientGame::Position& position)
{
    os << "(" << position.cell.x << ", " << position.cell.y << ", " <<
        position.depth << ")";
    return os;
}

const GameState& ClientGame::coreState() const
{
    return _coreState;
}

int ClientGame::activePlayer() const
{
    return _activePlayer;
}

const Cell& ClientGame::cell(int x, int y) const
{
    return _coreState.map[x + 2][y + 2];
}

TypeCellEnum ClientGame::cellType(int x, int y) const
{
    return TypeCellEnum(_coreState.map[x + 2][y + 2].type.type);
}

std::set<ClientGame::Cell> ClientGame::validMoves() const
{
    if (_state != State::SelectTarget) {
        return {};
    }

    std::set<Cell> cells;
    for (const auto& pair : _validMoves) {
        cells.insert(pair.first);
    }
    return cells;
}

void ClientGame::startLocalGame(int playerCount)
{
    assert(playerCount >= 1 && playerCount <= 4);

    _game = std::make_unique<LocalGame>();
    _coreState = _game->gameState();
    _state = State::SelectSubject;
    _activePlayer = 0;
    _game->newGame(playerCount);
}

void ClientGame::startRemoteGame()
{
    _game = std::make_unique<RemoteGame>();
    _coreState = _game->gameState();
    _state = State::SelectSubject;
    _game->newGame();
}

void ClientGame::update()
{
    if (_game) {
        _coreState = _game->gameState();
    }
}

void ClientGame::activateCell(const Cell& cell)
{
    switch (_state) {
        case State::SelectSubject:
        {
            const auto& coreCell = this->cell(cell.x, cell.y);
            if (std::ranges::find_if(
                    coreCell.pirate,
                    [this] (int pirateId) {
                        return _coreState.pirates.at(pirateId).id_player == activePlayer();
                    }) == coreCell.pirate.end()) {
                break;
            }

            auto highestPirate = std::ranges::max_element(
                coreCell.pirate, {},
                [this] (int i) {
                    if (_coreState.pirates.at(i).id_player != activePlayer()) {
                        return -1;
                    }

                    return _coreState.pirates.at(i).place.z;
                });

            _movingPirate = *highestPirate;
            _validMoves.clear();
            for (const Point& point : _game->legalSteps(_movingPirate)) {
                _validMoves[cellFromPoint(point)] = point.z;
            }

            const auto& piratePoint =
                _coreState.pirates.at(_movingPirate).place;
            const auto& pirateCell =
                _coreState.map[piratePoint.x][piratePoint.y];
            if (!_coreState.pirates.at(_movingPirate).money &&
                    pirateCell.money > 0) {
                _validMoves.emplace(cell, 0);
            }

            _state = State::SelectTarget;
            break;
        }

        case State::SelectTarget:
        {
            auto cellDepth = _validMoves.find(cell);

            // If activated cell is outside of valid set, consider selection
            // cancelled, and return to target selection state.
            if (cellDepth == _validMoves.end()) {
                _state = State::SelectSubject;
                return;
            }

            const auto& movingPiratePoint =
                _coreState.pirates.at(_movingPirate).place;
            auto movingPirateCell = cellFromPoint(movingPiratePoint);
            const auto& movingPirateMapPoint =
                _coreState.map[movingPiratePoint.x][movingPiratePoint.y];

            auto response = ResponseType{};
            if (cell == movingPirateCell && movingPirateMapPoint.money > 0) {
                response = _game->useMoney(_movingPirate);
            } else {
                response = _game->makeTurn(
                    _movingPirate, makePoint(cell, cellDepth->second));
            }

            switch (response) {
                case IllegalTurn:
                    throw std::runtime_error("ClientGame: illegal turn");
                    break;

                case Ok:
                    _activePlayer = (_activePlayer + 1) % PlayerCount;
                    _state = State::SelectSubject;
                    break;

                case WaitTurn:
                    _validMoves.clear();
                    for (const Point& point : _game->legalSteps(_movingPirate)) {
                        _validMoves[cellFromPoint(point)] = point.z;
                    }
                    break;
            }
            break;
        }
    }
}

ClientGame::Cell cellFromPoint(const Point& point)
{
    return {point.x - 2, point.y - 2};
}

ClientGame::Cell pirateCell(const Pirate& pirate)
{
    return cellFromPoint(pirate.place);
}
