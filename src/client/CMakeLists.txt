add_executable(client
    app.cpp
    client_game.cpp
    config.cpp
    field.cpp
    geometry.cpp
    gui.cpp
    log.cpp
    main.cpp
    resources.cpp
    timer.cpp
    universal_game.cpp
    view.cpp
    window.cpp
)
target_link_libraries(client
    PRIVATE
        asio
        client-server-api
        core
        data
        resource-ids
        sdl
        x

        yaml-cpp
)
add_dependencies(client assets)

add_custom_command(
    COMMENT "copy jackal.yaml custom command"
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/jackal.yaml
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/jackal.yaml
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/jackal.yaml
        ${CMAKE_CURRENT_BINARY_DIR}/jackal.yaml
)
add_custom_target(
    copy-jackal-config
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/jackal.yaml
)
add_dependencies(client copy-jackal-config)

add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        -t $<TARGET_FILE_DIR:client>
            $<TARGET_RUNTIME_DLLS:client>
            "${CMAKE_CURRENT_SOURCE_DIR}/jackal.yaml"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:client>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_BINARY_DIR}/assets/jackal.data
        $<TARGET_FILE_DIR:client>/jackal.data
    COMMAND_EXPAND_LISTS
)

install(TARGETS client)
install(
    FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/jackal.yaml"
        "${PROJECT_BINARY_DIR}/assets/jackal.data"
    TYPE BIN
)