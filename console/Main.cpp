#include "Console.hpp"
#include <boost/log/trivial.hpp>
#include <filesystem>

int main(int argc, char* argv[])
{
    // Travel upwards to directory containing resources
    constexpr int max_depth = 5;
    bool found = false;
    for (int i = 0; i < max_depth; ++i) {
        if (std::filesystem::exists("resources")) {
            found = true;
            break;
        }

        std::filesystem::current_path("..");
    }

    if (!found) {
        BOOST_LOG_TRIVIAL(error) << "Unable to find resources directory";
        return 1;
    }

    SetTraceLogLevel(LOG_WARNING);

    BOOST_LOG_TRIVIAL(info) << "Initializing temporary window";
    InitWindow(640, 480, "Temporary window");
    int width = GetMonitorWidth(0);
    int height = GetMonitorHeight(0);
    RlCloseWindow();

    BOOST_LOG_TRIVIAL(info) << "Initializing window with resolution " << width << "x" << height;
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(width, height, "Raspberry Console");

    BOOST_LOG_TRIVIAL(info) << "Initializing raspberry console";
    Console console;

    BOOST_LOG_TRIVIAL(info) << "Running raspberry console";
    console.run();

    BOOST_LOG_TRIVIAL(info) << "Closing window";
    RlCloseWindow();

    return 0;
}
