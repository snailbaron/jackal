#include "config.hpp"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <functional>

namespace {

void iterateConfig(auto& config, YAML::Node yaml, const auto& f)
{
    f(config.window.title, yaml["window"]["title"]);
    f(config.window.width, yaml["window"]["width"]);
    f(config.window.height, yaml["window"]["height"]);
    f(config.window.fullscreen, yaml["window"]["fullscreen"]);
    f(config.field.backgroundColor, yaml["field"]["background_color"]);
    f(config.field.gridColor, yaml["field"]["grid_color"]);
    f(config.field.gridWidth, yaml["field"]["grid_width"]);
    f(config.numberOfPlayers, yaml["number_of_players"]);
    f(config.fps, yaml["fps"]);
}

} // namespace

namespace YAML {

template <> struct convert<SDL_Color> {
    static Node encode(const SDL_Color& color)
    {
        auto node = Node{};
        node.push_back(color.r);
        node.push_back(color.g);
        node.push_back(color.b);
        if (color.a != 255) {
            node.push_back(color.a);
        }
        return node;
    }

    static bool decode(const Node& node, SDL_Color& color)
    {
        bool good = node.IsSequence() && node.size() >= 3 && node.size() <= 4 &&
            std::ranges::all_of(node, [] (const Node& node) {
                return node.IsScalar() &&
                    node.as<int>() >= 0 && node.as<int>() <= 255;
            });
        if (!good) {
            return false;
        }

        color.r = node[0].as<uint8_t>();
        color.g = node[1].as<uint8_t>();
        color.b = node[2].as<uint8_t>();
        color.a = 255;
        if (node.size() >= 4) {
            color.a = node[3].as<uint8_t>();
        }
        return true;
    }
};

} // namespace YAML

Config loadConfig(const std::filesystem::path& path)
{
    auto yaml = YAML::LoadFile(path.string());
    auto config = Config{};

    iterateConfig(
        config, yaml,
        [] <class T> (T& value, YAML::Node node) {
            if (node.IsDefined()) {
                value = node.as<T>();
            }
        });

    return config;
}

void saveConfig(const Config& config, const std::filesystem::path& path)
{
    auto yaml = YAML::Node{};
    iterateConfig(
        config, yaml,
        [] <class T> (const T& value, YAML::Node node) {
            node = value;
        });

    auto output = std::ofstream{path};
    output.exceptions(std::ios::badbit | std::ios::failbit);
    output << yaml;
    output.close();
}
