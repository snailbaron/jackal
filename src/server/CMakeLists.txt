add_custom_command(
    COMMENT "generating gRPC API"
    DEPENDS
        protobuf::protoc
        grpc_cpp_plugin
        server.proto
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/server.pb.h"
        "${CMAKE_CURRENT_BINARY_DIR}/server.pb.cc"
        "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.h"
        "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc"
    COMMAND $<TARGET_FILE:protobuf::protoc>
        --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        --plugin=protoc-gen-grpc=$<TARGET_FILE:grpc_cpp_plugin>
        server.proto
)

add_library(server
    server.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc"
)
target_link_libraries(server PRIVATE core)
