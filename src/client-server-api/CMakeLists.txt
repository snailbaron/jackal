add_custom_command(
    COMMENT "generating protobuf api"
    DEPENDS
        protoc
        "${CMAKE_CURRENT_SOURCE_DIR}/api.proto"
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/api.pb.h"
        "${CMAKE_CURRENT_BINARY_DIR}/api.pb.cc"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND $<TARGET_FILE:protoc>
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        --fatal_warnings
        api.proto
)

add_library(client-server-api
    client-server-api.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/api.pb.cc"
)
target_include_directories(client-server-api PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
target_link_libraries(client-server-api
    PUBLIC libprotobuf core x asio
)