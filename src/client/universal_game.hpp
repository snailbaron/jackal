#pragma once

#include <JackalGame.h>

#include <api.pb.h>

#include <asio.hpp>
#include <asio/experimental/coro.hpp>

#include <chrono>
#include <memory>
#include <mutex>
#include <optional>

class UniversalGame {
public:
    virtual ~UniversalGame() = default;

    virtual void update() {}

    virtual void newGame() = 0;
    virtual const GameState& gameState() = 0;
    virtual std::vector<Point> legalMoves(int pirateId) = 0;
    virtual ResponseType makeTurn(int pirateId, const Point& targetPoint) = 0;
    virtual ResponseType useMoney(int pirateId) = 0;
};

class LocalGame : public UniversalGame {
public:
    LocalGame();

    void newGame() override;
    const GameState& gameState() override;
    std::vector<Point> legalMoves(int pirateId) override;
    ResponseType makeTurn(int pirateId, const Point& targetPoint) override;
    ResponseType useMoney(int pirateId) override;

private:
    JackalGame _game;
    std::optional<GameState> _state;
};

class UpdateReader {
public:
    UpdateReader(asio::ip::tcp::socket& socket);

    std::optional<proto::Update> operator()();

private:
    std::optional<proto::Update> tryReadUpdate();

    asio::ip::tcp::socket& _socket;
    std::array<char, 4> _sizeBuffer;
    std::string _messageBuffer;
    size_t _sizeBytesRead = 0;
    size_t _messageBytesRead = 0;
};

class RemoteGame : public UniversalGame {
public:
    RemoteGame();

    void update() override;

    void newGame() override;
    const GameState& gameState() override;
    std::vector<Point> legalMoves(int pirateId) override;
    ResponseType makeTurn(int pirateId, const Point& targetPoint) override;
    ResponseType useMoney(int pirateId) override;

private:
    asio::io_context _ioContext;
    asio::ip::tcp::socket _socket;
    UpdateReader _updateReader;

    GameState _gameState;
    std::vector<std::vector<Point>> _allLegalMoves;

    asio::experimental::coro<> _readServerUpdates;
};