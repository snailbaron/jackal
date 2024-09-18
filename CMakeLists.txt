cmake_minimum_required(VERSION 3.24)

project(jackal)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

add_subdirectory(deps)

add_subdirectory(src)
