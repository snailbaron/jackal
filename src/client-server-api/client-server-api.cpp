#include "client-server-api.hpp"

#include <x.hpp>

std::string encodeSize(uint32_t size)
{
    namespace io = google::protobuf::io;

    auto string = std::string{};

    auto rawStream = io::StringOutputStream{&string};
    auto codedStream = io::CodedOutputStream{&rawStream};
    codedStream.WriteLittleEndian32(size);

    return string;
}

uint32_t decodeSize(const std::array<char, 4>& encodedSize)
{
    namespace io = google::protobuf::io;

    auto rawStream = io::ArrayInputStream(encodedSize.data(), encodedSize.size());
    auto codedStream = io::CodedInputStream{&rawStream};

    auto size = uint32_t{};
    if (!codedStream.ReadLittleEndian32(&size)) {
        throw x::Error{"decodeSize failed"};
    }

    return size;
}

std::string serializeWithSize(const google::protobuf::Message& proto)
{
    auto messageString = proto.SerializeAsString();
    auto sizeString = encodeSize(messageString.size());
    return sizeString + messageString;
}

proto::TimeState encode(const TimeState& timeState)
{
    auto protoTimeState = proto::TimeState{};
    protoTimeState.set_day(timeState.day);
    protoTimeState.set_step(timeState.step);
    return protoTimeState;
}

TimeState decode(const proto::TimeState& protoTimeState)
{
    return TimeState{
        .day = protoTimeState.day(),
        .step = protoTimeState.step(),
    };
}

proto::Point encode(const Point& point)
{
    auto protoPoint = proto::Point{};
    protoPoint.set_x(point.x);
    protoPoint.set_y(point.y);
    protoPoint.set_z(point.z);
    return protoPoint;
}

Point decode(const proto::Point& protoPoint)
{
    return Point{
        .x = protoPoint.x(),
        .y = protoPoint.y(),
        .z = protoPoint.z(),
    };
}

proto::Pirate encode(const Pirate& pirate)
{
    auto protoPirate = proto::Pirate{};
    protoPirate.set_is_alive(pirate.live);
    protoPirate.set_has_money(pirate.money);
    protoPirate.set_move_type((proto::MoveType)pirate.movie);
    *protoPirate.mutable_place() = encode(pirate.place);
    protoPirate.set_player_id(pirate.id_player);
    return protoPirate;
}

Pirate decode(const proto::Pirate& protoPirate)
{
    return Pirate{
        .live = protoPirate.is_alive(),
        .money = protoPirate.has_money(),
        .movie = (MovieType)protoPirate.move_type(),
        .place = decode(protoPirate.place()),
        .id_player = protoPirate.player_id(),
    };
}

proto::CellList encode(const Cell& cell)
{
    auto protoCellList = proto::CellList{};

    for (const Cell* p = &cell; p; p = p->next) {
        auto protoCell = protoCellList.add_cells();
        protoCell->set_type((proto::CellType)p->type.type);
        protoCell->set_money(p->money);
        for (int pirateId : p->pirate) {
            protoCell->add_pirates(pirateId);
        }
    }

    return protoCellList;
}

Cell decode(const proto::CellList& protoCellList)
{
    auto fillCellData = [] (Cell* cell, const proto::Cell& protoCell) {
        cell->type = protoCell.type();
        cell->money = protoCell.money();
        for (auto pirateId : protoCell.pirates()) {
            cell->pirate.push_back(pirateId);
        }
    };

    if (protoCellList.cells().empty()) {
        throw x::Error{"proto::CellList is empty"};
    }

    auto firstCell = Cell{};
    fillCellData(&firstCell, protoCellList.cells().at(0));

    Cell* currentCell = &firstCell;
    for (size_t i = 1; i < protoCellList.cells().size(); i++) {
        currentCell->next = new Cell;
        currentCell = currentCell->next;
        fillCellData(currentCell, protoCellList.cells().at(i));
    }

    return firstCell;
}

proto::GameState encode(const GameState& gameState)
{
    auto protoGameState = proto::GameState{};

    for (const auto& pirate : gameState.pirates) {
        *protoGameState.add_pirates() = encode(pirate);
    }

    auto protoMap = protoGameState.mutable_map();
    protoMap->set_size1(sizeof(gameState.map) / sizeof(gameState.map[0]));
    protoMap->set_size2(sizeof(gameState.map[0]) / sizeof(gameState.map[0][0]));
    for (const auto& cellRow : gameState.map) {
        for (const auto& cell : cellRow) {
            *protoMap->add_cell_lists() = encode(cell);
        }
    }

    for (const auto& pirateId : gameState.movie_pirate) {
        protoGameState.add_movable_pirate_ids(pirateId);
    }

    for (const auto& shipPoint : gameState.ships) {
        *protoGameState.add_ship_points() = encode(shipPoint);
    }

    *protoGameState.mutable_current_time() = encode(gameState.cur_time);

    return protoGameState;
}

GameState decode(const proto::GameState& protoGameState)
{
    auto gameState = GameState{};

    for (const auto& protoPirate : protoGameState.pirates()) {
        gameState.pirates.push_back(decode(protoPirate));
    }

    int k = 0;
    for (size_t i = 0; i < protoGameState.map().size1(); i++) {
        for (size_t j = 0; j < protoGameState.map().size2(); j++) {
            gameState.map[i][j] = decode(protoGameState.map().cell_lists(k++));
        }
    }

    for (const auto& pirateId : protoGameState.movable_pirate_ids()) {
        gameState.movie_pirate.push_back(pirateId);
    }

    for (size_t i = 0; i < protoGameState.ship_points().size(); i++) {
        const auto& shipPoint = protoGameState.ship_points(i);
        gameState.ships[i] = decode(shipPoint);
    }

    gameState.cur_time = decode(protoGameState.current_time());

    return gameState;
}

proto::TurnDescriptor encode(const TurnDescript& turnDescriptor)
{
    auto protoTurnDescriptor = proto::TurnDescriptor{};
    protoTurnDescriptor.set_pirate_id(turnDescriptor.pirate);
    *protoTurnDescriptor.mutable_to() = encode(turnDescriptor.to);
    *protoTurnDescriptor.mutable_from() = encode(turnDescriptor.from);
    protoTurnDescriptor.set_cell_to((proto::CellType)turnDescriptor.cell_to.type);
    protoTurnDescriptor.set_money_event((proto::MoneyEvent)turnDescriptor.money_event);
    protoTurnDescriptor.set_pirate_event((proto::PirateEvent)turnDescriptor.pirate_event);
    return protoTurnDescriptor;
}

TurnDescript decode(const proto::TurnDescriptor& protoTurnDescriptor)
{
    auto turnDescriptor = TurnDescript{};
    turnDescriptor.pirate = protoTurnDescriptor.pirate_id();
    turnDescriptor.to = decode(protoTurnDescriptor.to());
    turnDescriptor.from = decode(protoTurnDescriptor.from());
    turnDescriptor.cell_to = protoTurnDescriptor.cell_to();
    turnDescriptor.money_event = (MoneyEventType)protoTurnDescriptor.money_event();
    turnDescriptor.pirate_event = (PirateEventType)protoTurnDescriptor.pirate_event();
    return turnDescriptor;
}

proto::ChangeMap encode(const ChangeMap& changeMap)
{
    auto protoChangeMap = proto::ChangeMap{};
    for (const auto& turn : changeMap.turns) {
        *protoChangeMap.add_turns() = encode(turn);
    }
    for (const auto& time : changeMap.time) {
        *protoChangeMap.add_time() = encode(time);
    }
    return protoChangeMap;
}

ChangeMap decode(const proto::ChangeMap& protoChangeMap)
{
    auto changeMap = ChangeMap{};
    for (const auto& protoTurn : protoChangeMap.turns()) {
        changeMap.turns.push_back(decode(protoTurn));
    }
    for (const auto& protoTime : protoChangeMap.time()) {
        changeMap.time.push_back(decode(protoTime));
    }
    return changeMap;
}

proto::AllLegalMoves encodeAllLegalMoves(
    const std::vector<std::vector<Point>>& allLegalMoves)
{
    auto protoAllLegalMoves = proto::AllLegalMoves{};
    for (const auto& pirateLegalMoves : allLegalMoves) {
        auto protoPirateLegalMoves = protoAllLegalMoves.add_pirate_legal_moves();
        for (const auto& point : pirateLegalMoves) {
            *protoPirateLegalMoves->add_points() = encode(point);
        }
    }
    return protoAllLegalMoves;
}

std::vector<std::vector<Point>> decodeAllLegalMoves(
    const proto::AllLegalMoves& protoAllLegalMoves)
{
    auto allLegalMoves = std::vector<std::vector<Point>>{};
    allLegalMoves.reserve(protoAllLegalMoves.pirate_legal_moves().size());
    for (const auto& protoPirateLegalMoves :
            protoAllLegalMoves.pirate_legal_moves()) {
        auto& pirateLegalMoves = allLegalMoves.emplace_back();
        pirateLegalMoves.reserve(protoPirateLegalMoves.points().size());
        for (const auto& protoPoint : protoPirateLegalMoves.points()) {
            pirateLegalMoves.push_back(decode(protoPoint));
        }
    }
    return allLegalMoves;
}