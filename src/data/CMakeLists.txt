add_custom_command(
    COMMENT "generating header for data flatbuffers schema"
    DEPENDS flatc schema.fbs
    OUTPUT include/schema_generated.h
    COMMAND $<TARGET_FILE:flatc> --cpp --scoped-enums -o ${CMAKE_CURRENT_BINARY_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/schema.fbs
)

add_library(data STATIC
    ${CMAKE_CURRENT_BINARY_DIR}/include/schema_generated.h
    data.cpp
)
target_link_libraries(data INTERFACE flatbuffers)
target_include_directories(data PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)
set_target_properties(data PROPERTIES LINKER_LANGUAGE CXX)
