#pragma once

#include <api.pb.h>

#include <JackalGame.h>

#include <asio.hpp>

#include <cstdint>
#include <string>

std::string encodeSize(uint32_t size);
uint32_t decodeSize(const std::array<char, 4>& size);

proto::TimeState encode(const TimeState& timeState);
TimeState decode(const proto::TimeState& protoTimeState);

proto::Point encode(const Point& point);
Point decode(const proto::Point& protoPoint);

proto::Pirate encode(const Pirate& pirate);
Pirate decode(const proto::Pirate& protoPirate);

proto::CellList encode(const Cell& cell);
Cell decode(const proto::CellList& protoCellList);

proto::GameState encode(const GameState& gameState);
GameState decode(const proto::GameState& protoGameState);

proto::TurnDescriptor encode(const TurnDescript& turnDescriptor);
TurnDescript decode(const proto::TurnDescriptor& protoTurnDescriptor);

proto::ChangeMap encode(const ChangeMap& changeMap);
ChangeMap decode(const proto::ChangeMap& protoChangeMap);

proto::AllLegalMoves encodeAllLegalMoves(
    const std::vector<std::vector<Point>>& allLegalMoves);
std::vector<std::vector<Point>> decodeAllLegalMoves(
    const proto::AllLegalMoves& protoAllLegalMoves);


std::string serializeWithSize(const google::protobuf::Message& proto);

template <std::derived_from<google::protobuf::Message> Message>
asio::awaitable<Message> readMessageWithSize(asio::ip::tcp::socket& socket)
{
    auto sizeBuffer = std::array<char, 4>{};
    co_await asio::async_read(socket, asio::buffer(sizeBuffer));
    auto size = decodeSize(sizeBuffer);

    auto messageBuffer = std::string(size, '\0');
    co_await asio::async_read(
        socket, asio::buffer(messageBuffer.data(), messageBuffer.size()));

    auto message = Message{};
    if (!message.ParseFromString(messageBuffer)) {
        throw std::runtime_error{"cannot parse protobuf message"};
    }

    co_return message;
}