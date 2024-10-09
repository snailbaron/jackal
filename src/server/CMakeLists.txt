add_custom_command(
    COMMENT "generating gRPC API"
    DEPENDS
        protobuf::protoc
        grpc_cpp_plugin
        "${CMAKE_CURRENT_SOURCE_DIR}/server.proto"
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/server.pb.h"
        "${CMAKE_CURRENT_BINARY_DIR}/server.pb.cc"
        "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.h"
        "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND $<TARGET_FILE:protobuf::protoc>
        --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        --plugin=protoc-gen-grpc=$<TARGET_FILE:grpc_cpp_plugin>
        server.proto
)

add_executable(server
    server.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc"
    "${CMAKE_CURRENT_BINARY_DIR}/server.pb.cc"
)
target_include_directories(server PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(server PRIVATE grpc++ protobuf::libprotobuf core)