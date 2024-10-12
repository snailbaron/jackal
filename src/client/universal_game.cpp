#include "universal_game.hpp"

#include <client-server-api.hpp>

#include <x.hpp>

#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>

#include <format>
#include <source_location>

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
    check(_stub->NewGame(&_context, proto::NewGameRequest{}, nullptr));
}

void RemoteGame::newGame(int playerCount)
{
    auto newGameRequest = proto::NewGameRequest{};
    newGameRequest.set_player_count(playerCount);
    check(_stub->NewGame(&_context, newGameRequest, nullptr));
}

GameState RemoteGame::gameState()
{
    auto protoGameState = proto::GameState{};
    check(_stub->GetGameState(
        &_context, proto::EmptyRequest{}, &protoGameState));
    return decode(protoGameState);
}

std::vector<Point> RemoteGame::legalSteps(int pirateId)
{
    auto points = std::vector<Point>{};

    auto request = proto::PirateIdRequest{};
    request.set_pirate_id(pirateId);

    auto reader = _stub->GetLegalSteps(&_context, request);
    for (auto protoPoint = proto::Point{}; reader->Read(&protoPoint); ) {
        points.push_back(decode(protoPoint));
    }

    return points;
}

ResponseType RemoteGame::makeTurn(int pirateId, const Point& targetPoint)
{
    auto request = proto::MakeTurnRequest{};
    request.set_pirate_id(pirateId);
    *request.mutable_target_point() = encode(targetPoint);

    auto response = proto::StatusResponse{};

    check(_stub->MakeTurn(&_context, request, &response));
    return (ResponseType)response.response_type();
}

ResponseType RemoteGame::useMoney(int pirateId)
{
    auto request = proto::PirateIdRequest{};
    request.set_pirate_id(pirateId);

    auto response = proto::StatusResponse{};

    check(_stub->UseMoney(&_context, request, &response));
    return (ResponseType)response.response_type();
}