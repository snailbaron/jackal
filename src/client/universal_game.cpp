#include "universal_game.hpp"

#include <client-server-api.hpp>

#include <format>
#include <iostream>
#include <source_location>
#include <string>
#include <thread>

using namespace std::chrono_literals;

namespace {


} // namespace

LocalGame::LocalGame()
{
    _game.NewGame();
}

void LocalGame::newGame()
{
    _game = JackalGame{};
    _game.NewGame();
}

const GameState& LocalGame::gameState()
{
    if (!_state) {
        _state = _game.GetGameState();
    }
    return _state.value();
}

std::vector<Point> LocalGame::legalMoves(int pirateId)
{
    return _game.GetLegalSteps(pirateId);
}

ResponseType LocalGame::makeTurn(int pirateId, const Point& targetPoint)
{
    _state.reset();
    return _game.Turn(pirateId, targetPoint);
}

ResponseType LocalGame::useMoney(int pirateId)
{
    _state.reset();
    return _game.UseMoney(pirateId);
}

UpdateReader::UpdateReader(asio::ip::tcp::socket& socket)
    : _socket(socket)
{}

std::optional<proto::Update> UpdateReader::operator()()
{
    auto update = tryReadUpdate();
    while (update) {
        auto next = tryReadUpdate();
        if (!next) {
            return update;
        }
        update = next;
    }

    return update;
}

std::optional<proto::Update> UpdateReader::tryReadUpdate()
{
    if (_socket.available()) {
        std::cerr << "some data available on update socket\n";
    }

    if (_sizeBytesRead < _sizeBuffer.size() && _socket.available()) {
        _sizeBytesRead += _socket.read_some(asio::buffer(
            _sizeBuffer.data() + _sizeBytesRead,
            _sizeBuffer.size() - _sizeBytesRead));

        if (_sizeBytesRead == _sizeBuffer.size()) {
            auto size = decodeSize(_sizeBuffer);
            _messageBuffer.resize(size);
        }
    }

    if (_sizeBytesRead == _sizeBuffer.size() && _socket.available()) {
        _messageBytesRead += _socket.read_some(asio::buffer(
            _messageBuffer.data() + _messageBytesRead,
            _messageBuffer.size() - _messageBytesRead));

        if (_messageBytesRead == _messageBuffer.size()) {
            _sizeBytesRead = 0;
            _messageBytesRead = 0;

            auto protoUpdate = proto::Update{};
            protoUpdate.ParseFromString(_messageBuffer);
            return protoUpdate;
        }
    }

    return {};
}

RemoteGame::RemoteGame()
    : _socket(_ioContext)
    , _updateReader(_socket)
{
    auto resolver = asio::ip::tcp::resolver{_ioContext};
    auto query = asio::ip::tcp::resolver::query{"localhost", "12345"};

    asio::connect(_socket, resolver.resolve(query));
}

void RemoteGame::update()
{
    if (auto update = _updateReader()) {
        std::cerr << "got an update from server\n";
        _gameState = decode(update->game_state());
        _allLegalMoves = decodeAllLegalMoves(update->all_legal_moves());
    }
}

void RemoteGame::newGame()
{
    auto request = proto::Request{};
    request.mutable_new_game();

    auto data = serializeWithSize(request);
    asio::write(_socket, asio::buffer(data));
}

const GameState& RemoteGame::gameState()
{
    return _gameState;
}

std::vector<Point> RemoteGame::legalMoves(int pirateId)
{
    return _allLegalMoves.at(pirateId);
}

ResponseType RemoteGame::makeTurn(int pirateId, const Point& targetPoint)
{
    auto request = proto::Request{};
    request.mutable_make_move()->set_pirate_id(pirateId);
    *request.mutable_make_move()->mutable_target_point() = encode(targetPoint);

    auto data = serializeWithSize(request);
    asio::write(_socket, asio::buffer(data));

    return ResponseType::Ok;
}

ResponseType RemoteGame::useMoney(int pirateId)
{
    auto request = proto::Request{};
    request.mutable_use_money()->set_pirate_id(pirateId);

    auto data = serializeWithSize(request);
    asio::write(_socket, asio::buffer(data));

    return ResponseType::Ok;
}