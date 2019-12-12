#include "Config.hpp"
#include "Console.hpp"
#include <filesystem>
#include <iostream>

int main(int argc, char* argv[])
{
    // Travel upwards to the directory containing resources
    constexpr int max_depth { 5 };
    bool found = false;
    for (int i = 0; i < max_depth; ++i) {
        if (std::filesystem::exists("resources")) {
            found = true;
            break;
        }

        std::filesystem::current_path("..");
    }

    if (!found) {
        std::cerr << "Unable to find resources directory\n";
        return 1;
    }

    if (!Config::instance().load()) {
        std::cerr << "Unable to load configuration file\n";
        return 1;
    }

    SetTraceLogLevel(LOG_WARNING);

    int width = Config::integer("window", "width");
    int height = Config::integer("window", "height");
    std::cerr << "Initializing window with resolution " << width << "x" << height << "\n";

    unsigned int flags = 0;
    if (Config::boolean("window", "fullscreen")) {
        flags |= FLAG_FULLSCREEN_MODE;
        std::cerr << "Enabling fullscreen\n";
    }

    if (Config::boolean("window", "vsync")) {
        flags |= FLAG_VSYNC_HINT;
        std::cerr << "Enabling vsync\n";
    }
    SetConfigFlags(flags);

    InitWindow(width, height, "Raspberry Console");

    std::cerr << "Initializing console\n";
    Console console;

    std::cerr << "Running console\n";
    console.run();

    std::cerr << "Exiting\n";
    RlCloseWindow();

    return 0;
}
