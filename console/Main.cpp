#include "Console.hpp"
#include <filesystem>
#include <iostream>

int main(int argc, char* argv[])
{
    // Travel upwards to directory containing resources
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

    SetTraceLogLevel(LOG_WARNING);

    constexpr int width { 1024 };
    constexpr int height { 768 };

    std::cerr << "Initializing window with resolution " << width << "x" << height << "\n";
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(width, height, "Raspberry Console");

    std::cerr << "Initializing raspberry console\n";
    Console console;

    std::cerr << "Running raspberry console\n";
    console.run();

    std::cerr << "Closing window\n";
    RlCloseWindow();

    return 0;
}
