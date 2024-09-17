#include "client_game.hpp"
#include <cassert>

namespace {

ClientGame::Cell cellFromPoint(const Point& point)
{
    return {point.x - 2, point.y - 2};
}

ClientGame::Position positionFromPoint(const Point& point)
{
    return {cellFromPoint(point), point.z};
}

Point makePoint(const ClientGame::Cell& cell, int depth)
{
    return {cell.x + 2, cell.y + 2, depth};
}

} // namespace

/**
 * ClientGame::Cell operations
 */

bool operator==(const ClientGame::Cell& left, const ClientGame::Cell& right)
{
    return std::tie(left.x, left.y) == std::tie(right.x, right.y);
}

bool operator!=(const ClientGame::Cell& left, const ClientGame::Cell& right)
{
    return !(left == right);
}

bool operator<(const ClientGame::Cell& left, const ClientGame::Cell& right)
{
    return std::tie(left.x, left.y) < std::tie(right.x, right.y);
}

bool operator>(const ClientGame::Cell& left, const ClientGame::Cell& right)
{
    return right < left;
}

bool operator<=(const ClientGame::Cell& left, const ClientGame::Cell& right)
{
    return left < right || left == right;
}

bool operator>=(const ClientGame::Cell& left, const ClientGame::Cell& right)
{
    return right <= left;
}

std::ostream& operator<<(std::ostream& os, const ClientGame::Cell& cell)
{
    os << "(" << cell.x << ", " << cell.y << ")";
    return os;
}

/**
 * ClientGame::Position operations
 */

bool operator==(const ClientGame::Position& left, const ClientGame::Position& right)
{
    return std::tie(left.cell, left.depth) == std::tie(right.cell, right.depth);
}

bool operator!=(const ClientGame::Position& left, const ClientGame::Position& right)
{
    return !(left == right);
}

bool operator<(const ClientGame::Position& left, const ClientGame::Position& right)
{
    return std::tie(left.cell, left.depth) < std::tie(right.cell, right.depth);
}

bool operator>(const ClientGame::Position& left, const ClientGame::Position& right)
{
    return right < left;
}

bool operator<=(const ClientGame::Position& left, const ClientGame::Position& right)
{
    return left < right || left == right;
}

bool operator>=(const ClientGame::Position& left, const ClientGame::Position& right)
{
    return right <= left;
}

std::ostream& operator<<(std::ostream& os, const ClientGame::Position& position)
{
    os << "(" << position.cell.x << ", " << position.cell.y << ", " <<
        position.depth << ")";
    return os;
}

/**
 * Game parameters
 */

const int ClientGame::FieldSize = 13;

/**
 * ClientGame::State
 */

enum class ClientGame::State {
    SelectSubject,
    ClarifySubject,
    SelectTarget,
};

/**
 * ClientGame
 */

ClientGame::ClientGame(std::shared_ptr<JackalGame> game)
    : _game(game)
    , _coreState(game->GetGameState())
    , _state(State::SelectSubject)
    , _playerCount(0)
    , _activePlayer(-1)
{ }

const GameState& ClientGame::coreState() const
{
    return _coreState;
}

int ClientGame::activePlayer() const
{
    return _activePlayer;
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

void ClientGame::startNewGame(int playerCount)
{
    assert(playerCount >= 1 && playerCount <= 4);
    _playerCount = playerCount;
    _activePlayer = 0;
    _game->NewGame(playerCount);
}

void ClientGame::update()
{
    _coreState = _game->GetGameState();
}

void ClientGame::activateCell(const Cell& cell)
{
    switch (_state) {
        case State::SelectSubject:
        {
            // Find all candidates for the move: z-coordinate => pirate ID
            std::map<int, int> candidates;
            for (int i : _coreState.movie_pirate) {
                const Point& piratePoint = _coreState.pirates[i].place;
                if (cellFromPoint(piratePoint) == cell) {
                    candidates[piratePoint.z] = i;
                }
            }

            // All candidates are on the same depth. Take any of them.
            if (candidates.size() == 1) {
                _movingPirate = candidates.begin()->second;
                _validMoves.clear();
                for (const Point& point : _game->GetLegalSteps(_movingPirate)) {
                    _validMoves[cellFromPoint(point)] = point.z;
                }
                _state = State::SelectTarget;
                return;
            }

            // There are candidates on different depths. Need to select one.
            if (candidates.size() > 1) {
                _moveCandidates.clear();
                for (const auto zPiratePair : candidates) {
                    _moveCandidates.push_back({
                        cell.x, cell.y, zPiratePair.first});
                }
                _state = State::ClarifySubject;
            }
            break;
        }

        case State::ClarifySubject:
        {
            // If a cell is activated when clarifying move subject, cancel
            // clarification, and return to move subject selection.
            _state = State::SelectSubject;
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

            ResponseType response = _game->Turn(
                _movingPirate, makePoint(cell, cellDepth->second));

            switch (response) {
                case IllegalTurn:
                    throw std::runtime_error("ClientGame: illegal turn");
                    break;

                case Ok:
                    _activePlayer = (_activePlayer + 1) % _playerCount;
                    _state = State::SelectSubject;
                    break;

                case WaitTurn:
                    _validMoves.clear();
                    for (const Point& point : _game->GetLegalSteps(_movingPirate)) {
                        _validMoves[cellFromPoint(point)] = point.z;
                    }
                    break;
            }
            break;
        }
    }
}

ClientGame::Cell pirateCell(const Pirate& pirate)
{
    return cellFromPoint(pirate.place);
}