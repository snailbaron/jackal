#pragma once

#include "sdl.hpp"

#include <filesystem>
#include <string>

struct Config {
    struct Window {
        std::string title = "Jackal";
        int width = 800;
        int height = 600;
        bool fullscreen = false;
    } window;

    struct Field {
        SDL_Color backgroundColor{200, 200, 200, 255};
        SDL_Color gridColor{100, 100, 100, 255};
        int gridWidth = 3;
    } field;

    int numberOfPlayers = 4;
    int fps = 60;
};

Config loadConfig(const std::filesystem::path& path);
void saveConfig(const Config& config, const std::filesystem::path& path);