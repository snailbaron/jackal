#include "universal_game.hpp"

#include <client-server-api.hpp>

#include <x.hpp>

#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>

#include <format>
#include <source_location>

using namespace std::chrono_literals;

namespace {

void check(
    const grpc::Status& status,
    std::source_location sl = std::source_location::current())
{
    if (!status.ok()) {
        throw x::Error{status.error_message(), sl};
    }
}

} // namespace

void LocalGame::newGame()
{
    _game.NewGame();
}

void LocalGame::newGame(int playerCount)
{
    _game.NewGame(playerCount);
}

GameState LocalGame::gameState()
{
    return _game.GetGameState();
}

std::vector<Point> LocalGame::legalSteps(int pirateId)
{
    return _game.GetLegalSteps(pirateId);
}

ResponseType LocalGame::makeTurn(int pirateId, const Point& targetPoint)
{
    return _game.Turn(pirateId, targetPoint);
}

ResponseType LocalGame::useMoney(int pirateId)
{
    return _game.UseMoney(pirateId);
}

RemoteGame::RemoteGame()
    : _channel(grpc::CreateChannel(
        "localhost:12345", grpc::InsecureChannelCredentials()))
    , _stub(proto::JackalService::NewStub(_channel))
{ }

void RemoteGame::newGame()
{
    auto context = grpc::ClientContext{};
    auto response = proto::EmptyResponse{};
    check(_stub->NewGame(&context, proto::NewGameRequest{}, &response));
}

void RemoteGame::newGame(int playerCount)
{
    auto context = grpc::ClientContext{};

    auto newGameRequest = proto::NewGameRequest{};
    newGameRequest.set_player_count(playerCount);

    auto response = proto::EmptyResponse{};

    check(_stub->NewGame(&context, newGameRequest, &response));
}

GameState RemoteGame::gameState()
{
    auto currentFrame = (Clock::now() - _startTime) / updateDelay;
    if (currentFrame >= _nextUpdateFrame) {
        _nextUpdateFrame = currentFrame + 1;

        auto context = grpc::ClientContext{};

        auto protoGameState = proto::GameState{};
        check(_stub->GetGameState(
            &context, proto::EmptyRequest{}, &protoGameState));
        _gameState = decode(protoGameState);
    }

    return _gameState;
}

std::vector<Point> RemoteGame::legalSteps(int pirateId)
{
    auto context = grpc::ClientContext{};

    auto points = std::vector<Point>{};

    auto request = proto::PirateIdRequest{};
    request.set_pirate_id(pirateId);

    auto reader = _stub->GetLegalSteps(&context, request);
    for (auto protoPoint = proto::Point{}; reader->Read(&protoPoint); ) {
        points.push_back(decode(protoPoint));
    }

    return points;
}

ResponseType RemoteGame::makeTurn(int pirateId, const Point& targetPoint)
{
    auto context = grpc::ClientContext{};

    auto request = proto::MakeTurnRequest{};
    request.set_pirate_id(pirateId);
    *request.mutable_target_point() = encode(targetPoint);

    auto response = proto::StatusResponse{};

    check(_stub->MakeTurn(&context, request, &response));
    return (ResponseType)response.response_type();
}

ResponseType RemoteGame::useMoney(int pirateId)
{
    auto context = grpc::ClientContext{};

    auto request = proto::PirateIdRequest{};
    request.set_pirate_id(pirateId);

    auto response = proto::StatusResponse{};

    check(_stub->UseMoney(&context, request, &response));
    return (ResponseType)response.response_type();
}

const RemoteGame::Clock::duration RemoteGame::updateDelay =
    std::chrono::duration_cast<Clock::duration>(0.5s);