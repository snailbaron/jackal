add_executable(server
    server.cpp
)
target_link_libraries(server PRIVATE client-server-api core)

install(TARGETS server)