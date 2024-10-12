#include "server.grpc.pb.h"

#include <client-server-api.hpp>
#include <JackalGame.h>

#include <grpcpp/server_builder.h>

#include <iostream>
#include <mutex>
#include <string>

class JackalServiceImpl final : public proto::JackalService::Service {
public:
    grpc::Status NewGame(
        grpc::ServerContext* context,
        const proto::NewGameRequest* request,
        proto::EmptyResponse* response) override
    {
        auto lock = std::lock_guard{_mutex};

        if (request->player_count() == 0) {
            _core.NewGame();
        } else {
            _core.NewGame(request->player_count());
        }

        return grpc::Status::OK;
    }

    grpc::Status GetGameState(
        grpc::ServerContext* context,
        const proto::EmptyRequest* request,
        proto::GameState* response) override
    {
        auto lock = std::lock_guard{_mutex};

        auto state = _core.GetGameState();
        *response = encode(state);

        return grpc::Status::OK;
    }


    grpc::Status GetChangesMap(
        grpc::ServerContext* context,
        const proto::TimeState* request,
        proto::ChangeMap* response) override
    {
        auto lock = std::lock_guard{_mutex};

        *response = encode(_core.GetChangesMap(decode(*request)));
        return grpc::Status::OK;
    }

    grpc::Status GetLegalSteps(
        grpc::ServerContext* context,
        const proto::PirateIdRequest* request,
        grpc::ServerWriter<proto::Point>* writer) override
    {
        auto lock = std::lock_guard{_mutex};

        for (const auto& point : _core.GetLegalSteps(request->pirate_id())) {
            writer->Write(encode(point));
        }
        return grpc::Status::OK;
    }

    grpc::Status MakeTurn(
        grpc::ServerContext* context,
        const proto::MakeTurnRequest* request,
        proto::StatusResponse* response) override
    {
        auto lock = std::lock_guard{_mutex};

        response->set_response_type(
            (proto::ResponseType)_core.Turn(
                request->pirate_id(), decode(request->target_point())));
        return grpc::Status::OK;
    }

    grpc::Status UseMoney(
        grpc::ServerContext* context,
        const proto::PirateIdRequest* request,
        proto::StatusResponse* response) override
    {
        auto lock = std::lock_guard{_mutex};

        response->set_response_type(
            (proto::ResponseType)_core.UseMoney(request->pirate_id()));
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