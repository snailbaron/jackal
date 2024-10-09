#include "server.grpc.pb.h"

#include <JackalGame.h>

#include <grpcpp/server_builder.h>

#include <iostream>
#include <mutex>
#include <string>

class JackalServiceImpl final : public proto::JackalService::Service {
public:
    JackalServiceImpl()
    {
        _core.NewGame();
    }

    grpc::Status GetGameState(
        grpc::ServerContext* context,
        const proto::Empty* request,
        proto::GameState* response) override
    {
        auto lock = std::lock_guard{_mutex};

        auto state = _core.GetGameState();

        for (const auto& pirate : state.pirates) {
            auto protoPirate = response->add_pirates();
            protoPirate->set_is_alive(pirate.live);
            protoPirate->set_has_money(pirate.money);
            protoPirate->set_move_type(proto::MoveType{pirate.movie});
            auto protoPlace = protoPirate->mutable_place();
            protoPlace->set_x(pirate.place.x);
            protoPlace->set_y(pirate.place.y);
            protoPlace->set_z(pirate.place.z);
            protoPirate->set_player_id(pirate.id_player);
        }

        for (const auto& row : state.map) {
            for (const auto& cell : row) {
                auto protoCell = response->add_map();
                protoCell->set_type(proto::CellType{cell.type.type});
                protoCell->set_money(cell.money);
                for (const auto& pirateId : cell.pirate) {
                    protoCell->add_pirate(pirateId);
                }

                size_t cellDepth = 1;
                for (auto p = cell.next; p; p = p->next) {
                    cellDepth++;
                }
                protoCell->set_depth(cellDepth);
            }
        }

        for (const auto& shipPoint : state.ships) {
            auto protoShip = response->add_ships();
            protoShip->set_x(shipPoint.x);
            protoShip->set_y(shipPoint.y);
            protoShip->set_z(shipPoint.z);
        }

        auto protoCurrentTime = response->mutable_current_time();
        protoCurrentTime->set_day(state.cur_time.day);
        protoCurrentTime->set_step(state.cur_time.step);

        return grpc::Status::OK;
    }

    grpc::Status GetLegalSteps(
        grpc::ServerContext* context,
        const proto::PirateId* request,
        grpc::ServerWriter<proto::Point>* writer) override
    {
        auto lock = std::lock_guard{_mutex};

        auto legalPoints = _core.GetLegalSteps(request->id());
        for (const auto& point : legalPoints) {
            auto protoPoint = proto::Point{};
            protoPoint.set_x(point.x);
            protoPoint.set_y(point.y);
            protoPoint.set_z(point.z);
            writer->Write(protoPoint);
        }

        return grpc::Status::OK;
    }

    grpc::Status MakeTurn(
        grpc::ServerContext* context,
        const proto::MakeTurnRequest* request,
        proto::Empty* response)
    {
        auto lock = std::lock_guard{_mutex};

        auto point = Point{
            .x = request->target_point().x(),
            .y = request->target_point().y(),
            .z = request->target_point().z(),
        };
        _core.Turn(request->pirate_id(), point);

        return grpc::Status::OK;
    }

    grpc::Status UseMoney(
        grpc::ServerContext* context,
        const ::proto::PirateId* request,
        proto::Empty* response)
    {
        auto lock = std::lock_guard{_mutex};

        _core.UseMoney(request->id());

        return grpc::Status::OK;
    }

private:
    JackalGame _core;
    std::mutex _mutex;
};

int main()
{
    auto serverAddress = std::string{"0.0.0.0:12345"};
    auto service = JackalServiceImpl{};

    auto builder = grpc::ServerBuilder{};
    builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    auto server = builder.BuildAndStart();
    std::cerr << "jackal server listening on " << serverAddress << "\n";
    server->Wait();
}