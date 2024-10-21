#include <JackalGame.h>

#include <client-server-api.hpp>

#include <asio.hpp>
#include <google/protobuf/io/coded_stream.h>

#include <array>
#include <bit>
#include <concepts>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

class PlayerIdGenerator {
public:
    PlayerIdGenerator()
        : _randomEngine(_randomDevice())
        , _dist('a', 'z')
    { }

    std::string operator()()
    {
        auto result = std::string(100, '\0');
        for (int i = 0; i < 100; i++) {
            result.at(i) = (char)_dist(_randomEngine);
        }
        return result;
    }

private:
    std::random_device _randomDevice;
    std::mt19937 _randomEngine;
    std::uniform_int_distribution<int> _dist;
};

class Server {
public:
    Server()
        : _acceptor(
            _ioContext,
            asio::ip::tcp::endpoint{asio::ip::tcp::v4(), 12345})
    {
        _core.NewGame();
        _updateData = updateData();

        processNewConnections();
    }

    void run()
    {
        _ioContext.run();
    }

    void processNewConnections()
    {
        std::cerr << "processing new connections\n";

        _acceptor.async_accept(
            _ioContext,
            [this] (const std::error_code& error, asio::ip::tcp::socket socket) {
                std::cerr << "accepted new connection\n";

                auto ptr = std::make_shared<asio::ip::tcp::socket>(
                    std::move(socket));

                _connections.push_back(ptr);
                std::cerr << "now have " << _connections.size() << " connections\n";

                asio::co_spawn(
                    _ioContext.get_executor(),
                    readRequests(ptr),
                    asio::detached);

                updateClient(ptr);

                processNewConnections();
            });

        std::cerr << "finished processing new connections\n";
    }

    asio::awaitable<void> readRequests(
        std::shared_ptr<asio::ip::tcp::socket> socket)
    {
        std::cerr << "reading requests from a new client\n";

        while (socket->is_open()) {
            auto request = co_await readMessageWithSize<proto::Request>(*socket);

            switch (request.request_case()) {
                case proto::Request::kMakeMove:
                    std::cerr << "request type: make a move\n";

                    _core.Turn(
                        request.make_move().pirate_id(),
                        decode(request.make_move().target_point()));

                    _updateData = updateData();
                    updateAllClients();

                    break;

                case proto::Request::kUseMoney:
                    std::cerr << "request type: use money\n";

                    _core.UseMoney(request.use_money().pirate_id());

                    _updateData = updateData();
                    updateAllClients();

                    break;

                case proto::Request::kNewGame:
                    std::cerr << "request type: new game\n";

                    _core = JackalGame{};
                    _core.NewGame();
                    _updateData = updateData();

                    break;

                case proto::Request::REQUEST_NOT_SET:
                    throw std::runtime_error{"request not set"};

                default:
                    throw std::runtime_error{"unexpected request variant"};
            }
        }

        std::erase(_connections, socket);
        std::cerr << "closed connection, now have " << _connections.size() <<
            " connections\n";
    }

private:
    [[nodiscard]] std::string updateData()
    {
        auto gameState = _core.GetGameState();

        auto allLegalMoves = std::vector<std::vector<Point>>{};
        allLegalMoves.reserve(gameState.pirates.size());
        for (size_t i = 0; i < gameState.pirates.size(); i++) {
            allLegalMoves.push_back(_core.GetLegalSteps(i));
        }

        auto protoUpdate = proto::Update{};
        *protoUpdate.mutable_game_state() = encode(gameState);
        *protoUpdate.mutable_all_legal_moves() = encodeAllLegalMoves(allLegalMoves);

        return serializeWithSize(protoUpdate);
    }

    void updateClient(std::shared_ptr<asio::ip::tcp::socket> socket)
    {
        std::cerr << "updating a client\n";
        asio::write(*socket, asio::buffer(_updateData));
    }

    void updateAllClients()
    {
        std::cerr << "updating all clients\n";
        for (const auto& socket : _connections) {
            updateClient(socket);
        }
    }

    asio::io_context _ioContext;
    asio::ip::tcp::acceptor _acceptor;
    std::vector<std::shared_ptr<asio::ip::tcp::socket>> _connections;

    PlayerIdGenerator _idGenerator;

    JackalGame _core;
    std::string _updateData;
};

int main(int, char*[]) try
{
    auto server = Server{};
    server.run();
} catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
}