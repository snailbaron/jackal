#pragma once

#include <server.pb.h>

#include <JackalGame.h>

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