#pragma once

#include <JackalGame.h>

#include <server.grpc.pb.h>

#include <grpcpp/channel.h>

#include <chrono>
#include <memory>

class UniversalGame {
public:
    virtual ~UniversalGame() = default;

    virtual void newGame() = 0;
    virtual void newGame(int playerCount) = 0;
    virtual GameState gameState() = 0;
    virtual std::vector<Point> legalSteps(int pirateId) = 0;
    virtual ResponseType makeTurn(int pirateId, const Point& targetPoint) = 0;
    virtual ResponseType useMoney(int pirateId) = 0;
};

class LocalGame : public UniversalGame {
public:
    void newGame() override;
    void newGame(int playerCount) override;
    GameState gameState() override;
    std::vector<Point> legalSteps(int pirateId) override;
    ResponseType makeTurn(int pirateId, const Point& targetPoint) override;
    ResponseType useMoney(int pirateId) override;

private:
    JackalGame _game;
};

class RemoteGame : public UniversalGame {
public:
    RemoteGame();

    void newGame() override;
    void newGame(int playerCount) override;
    GameState gameState() override;
    std::vector<Point> legalSteps(int pirateId) override;
    ResponseType makeTurn(int pirateId, const Point& targetPoint) override;
    ResponseType useMoney(int pirateId) override;

private:
    using Clock = std::chrono::system_clock;
    static const Clock::duration updateDelay;

    std::shared_ptr<grpc::Channel> _channel;
    std::unique_ptr<proto::JackalService::Stub> _stub;

    GameState _gameState;
    Clock::time_point _startTime = Clock::now();
    size_t _nextUpdateFrame = 0;
};