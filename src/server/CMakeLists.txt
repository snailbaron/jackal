add_executable(server
    server.cpp
)
target_link_libraries(server PRIVATE core client-server-api asio)

install(TARGETS server)