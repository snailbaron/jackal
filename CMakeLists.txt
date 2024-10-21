cmake_minimum_required(VERSION 3.24)

project(jackal)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# This option is required. Some of the subprojects set it, so leaving it unset
# here may lead to inconsistent behavior between builds.
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_subdirectory(assets)
add_subdirectory(deps)
add_subdirectory(src)