add_library(core STATIC
    Cell.cpp
    ChangeMap.cpp
    GameState.cpp
    JackalGame.cpp
    main.cpp
    Point.cpp
    TimeState.cpp
)

add_executable(jackal-cli
    main.cpp
)
target_link_libraries(jackal-cli PRIVATE core)