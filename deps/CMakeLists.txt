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

# Disable asm in grcp build
set(OPENSSL_NO_ASM TRUE)
set(protobuf_INSTALL FALSE)
set(utf8_range_ENABLE_INSTALL FALSE)

add_subdirectory(flatbuffers)
add_subdirectory(grpc)
add_subdirectory(SDL)
add_subdirectory(SDL_image)
add_subdirectory(SDL_ttf)
add_subdirectory(yaml-cpp)

add_subdirectory(arg)