#include <schema_generated.h>

#include <x.hpp>

#include <arg.hpp>

#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::string toCamelCase(const std::string& s)
{
    auto output = std::ostringstream{};

    bool toUpper = true;
    for (char c : s) {
        if (c == '-' || c == '_') {
            toUpper = true;
            continue;
        }

        if (toUpper) {
            c = (char)std::toupper(c);
            toUpper = false;
        }
        output << c;
    }

    return output.str();
}

int main(int argc, char* argv[])
{
    arg::helpKeys("-h", "--help");
    auto base = arg::option<std::filesystem::path>()
        .keys("--base")
        .metavar("DIR")
        .markRequired()
        .help("path to base directory with assets");
    auto sprites = arg::multiOption<std::filesystem::path>()
        .keys("--sprite")
        .metavar("FILE")
        .help("path to sprite file");
    auto fonts = arg::multiOption<std::filesystem::path>()
        .keys("--font")
        .metavar("FILE")
        .help("path to font file");
    auto outputHeaderPath = arg::option<std::filesystem::path>()
        .keys("--header")
        .metavar("FILE")
        .help("path to output header file");
    auto outputDataPath = arg::option<std::filesystem::path>()
        .keys("--data")
        .metavar("FILE")
        .help("path to output data file");
    arg::parse(argc, argv);

    auto builder = flatbuffers::FlatBufferBuilder{};

    auto spriteNames = std::vector<std::string>{};
    auto spriteOffsets = std::vector<flatbuffers::Offset<fb::Sprite>>{};
    for (const auto& spriteRelativePath : sprites) {
        auto path = base / spriteRelativePath;

        auto spriteName = toCamelCase(path.stem().string());
        spriteNames.push_back(spriteName);
        auto spriteContents = x::readFile<uint8_t>(path);

        auto nameOffset = builder.CreateString(spriteName);
        auto contentsOffset = builder.CreateVector(spriteContents);

        spriteOffsets.push_back(
            fb::CreateSprite(builder, nameOffset, contentsOffset));
    }

    auto fontNames = std::vector<std::string>{};
    auto fontOffsets = std::vector<flatbuffers::Offset<fb::Font>>{};
    for (const auto& fontRelativePath : fonts) {
        auto path = base / fontRelativePath;

        auto fontName = toCamelCase(path.stem().string());
        fontNames.push_back(fontName);
        auto fontContents = x::readFile<uint8_t>(path);

        auto nameOffset = builder.CreateString(fontName);
        auto contentsOffset = builder.CreateVector(fontContents);

        fontOffsets.push_back(
            fb::CreateFont(builder, nameOffset, contentsOffset));
    }

    auto dataOffset = fb::CreateData(
        builder,
        builder.CreateVector(spriteOffsets),
        builder.CreateVector(fontOffsets));

    builder.Finish(dataOffset, "JCKL");

    builder.GetBufferSpan();

    x::writeFile(builder.GetBufferSpan(), outputDataPath);

    std::filesystem::create_directories(outputHeaderPath->parent_path());
    auto header = std::ofstream{*outputHeaderPath};
    header.exceptions(std::ios::badbit | std::ios::failbit);
    header <<
        "#pragma once\n" <<
        "\n" <<
        "namespace r {\n" <<
        "\n"
        "enum class Font {\n";
    for (const auto& fontName : fontNames) {
        header << "    " << fontName << ",\n";
    }
    header <<
        "};\n" <<
        "\n" <<
        "enum class Sprite {\n";
    for (const auto& spriteName : spriteNames) {
        header << "    " << spriteName << ",\n";
    }
    header <<
        "};\n" <<
        "\n" <<
        "} // namespace r\n";
    header.close();
}
