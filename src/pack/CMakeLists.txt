add_executable(pack
    main.cpp
)
target_link_libraries(pack PRIVATE arg data x flatbuffers)

add_custom_command(TARGET pack POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        -t $<TARGET_FILE_DIR:pack> $<TARGET_RUNTIME_DLLS:pack>
    COMMAND_EXPAND_LISTS
)
