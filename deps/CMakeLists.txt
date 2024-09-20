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
# link it statically.
set(YAML_BUILD_SHARED_LIBS FALSE)

add_subdirectory(flatbuffers)
add_subdirectory(SDL)
add_subdirectory(SDL_image)
add_subdirectory(SDL_ttf)
add_subdirectory(yaml-cpp)

add_subdirectory(arg)