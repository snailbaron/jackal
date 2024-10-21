include(ExternalProject)

set(SDL2IMAGE_AVIF OFF)
set(SDL2IMAGE_BMP ON)
set(SDL2IMAGE_GIF OFF)
set(SDL2IMAGE_JPG OFF)
set(SDL2IMAGE_JXL OFF)
set(SDL2IMAGE_LBM OFF)
set(SDL2IMAGE_PCX OFF)
set(SDL2IMAGE_PNG ON)
set(SDL2IMAGE_PNM OFF)
set(SDL2IMAGE_QOI OFF)
set(SDL2IMAGE_SVG OFF)
set(SDL2IMAGE_TGA OFF)
set(SDL2IMAGE_TIF OFF)
set(SDL2IMAGE_WEBP OFF)
set(SDL2IMAGE_XCF OFF)
set(SDL2IMAGE_XPM OFF)
set(SDL2IMAGE_XV OFF)

set(SDL2IMAGE_VENDORED TRUE)
set(SDL2TTF_VENDORED TRUE)

# yaml-cpp's messy use of dllexport causes MSVC to freak out (C4251). Always
# link it statically. We have to use a cache variable here, otherwise yaml-cpp's
# CMakeLists.txt ignores it on first build due to
# https://cmake.org/cmake/help/latest/policy/CMP0077.html.
set(YAML_BUILD_SHARED_LIBS FALSE CACHE BOOL "build yaml-cpp shared libraries")

set(protobuf_BUILD_TESTS FALSE)
set(protobuf_BUILD_LIBUPB FALSE)
set(protobuf_INSTALL FALSE)
set(protobuf_MSVC_STATIC_RUNTIME FALSE)

set(FLATBUFFERS_INSTALL FALSE)

add_subdirectory(flatbuffers)
add_subdirectory(protobuf)
add_subdirectory(SDL)
add_subdirectory(SDL_image)
add_subdirectory(SDL_ttf)
add_subdirectory(yaml-cpp)

add_subdirectory(arg)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE asio/asio/include)