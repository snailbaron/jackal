if(MSVC)
    add_compile_options(/WX)
else()
    add_compile_options(-Werror)
endif()

add_executable(client
    app.cpp
    client_game.cpp
    exceptions.cpp
    field.cpp
    geometry.cpp
    gui.cpp
    log.cpp
    main.cpp
    resources.cpp
    sdl_wrapper.cpp
    view.cpp
    window.cpp
)
target_link_libraries(client PRIVATE
    core

    nlohmann_json::nlohmann_json
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)

add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        -t $<TARGET_FILE_DIR:client>
            $<TARGET_RUNTIME_DLLS:client>
            "${CMAKE_CURRENT_SOURCE_DIR}/jackal.json"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:client>/assets"
    COMMAND_EXPAND_LISTS
)