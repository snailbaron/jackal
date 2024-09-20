macro(resources)
    set(options "")
    set(one_value_args "")
    set(multi_value_args FONTS SPRITES)
    cmake_parse_arguments(RES
        "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    set(font_args "")
    foreach(i ${RES_FONTS})
        list(APPEND font_args "--font")
        list(APPEND font_args "${i}")
    endforeach()

    set(sprite_args "")
    foreach(i ${RES_SPRITES})
        list(APPEND sprite_args "--sprite")
        list(APPEND sprite_args "${i}")
    endforeach()

    add_custom_command(
        COMMENT "packing resources"
        DEPENDS ${RES_SPRITES} ${RES_FONTS} pack
        OUTPUT
            jackal.data
            include/resource-ids.hpp
        COMMAND $<TARGET_FILE:pack>
            --base ${CMAKE_CURRENT_SOURCE_DIR}
            --data ${CMAKE_CURRENT_BINARY_DIR}/jackal.data
            --header ${CMAKE_CURRENT_BINARY_DIR}/include/resource-ids.hpp
            ${sprite_args}
            ${font_args}
    )

    add_custom_target(assets
        DEPENDS
            ${CMAKE_CURRENT_BINARY_DIR}/jackal.data
            ${CMAKE_CURRENT_BINARY_DIR}/include/resource-ids.hpp
    )
endmacro()

resources(
    FONTS
        fonts/fyodor/truetype/Fyodor-Bold.ttf

    SPRITES
        tiles/airplane.png
        tiles/arrow.png
        tiles/arrow_base.png
        tiles/ballon.png
        tiles/baloon.png
        tiles/cannibal.png
        tiles/cannon_down.png
        tiles/cannon_left.png
        tiles/cannon_right.png
        tiles/cannon_up.png
        tiles/closed.png
        tiles/crocodile.png
        tiles/difficult_terrain.png
        tiles/fort.png
        tiles/grass.png
        tiles/horse.png
        tiles/ice.png
        tiles/native_woman.png
        tiles/pirate_blue.png
        tiles/pirate_green.png
        tiles/pirate_none.png
        tiles/pirate_red.png
        tiles/pirate_yellow.png
        tiles/pitfall.png
        tiles/root.png
        tiles/rum.png
        tiles/treasure.png
        tiles/water.png
)

add_library(resource-ids resource-ids.cpp)
add_dependencies(resource-ids assets)
target_include_directories(
    resource-ids PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)
