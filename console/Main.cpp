// Fixes build errors on Windows, must be included first
#include "Windows.hpp"

#include "Config.hpp"
#include "Console.hpp"
#include "Logging.hpp"
#include <filesystem>

int main(int argc, char* argv[])
{
    spdlog::set_level(spdlog::level::debug);

#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR) {
        spdlog::error("Error at WSAStartup()");
        return 1;
    }
#endif

    spdlog::debug("Looking for root directory");
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
        spdlog::error("Unable to find resources directory");
        return 1;
    }

    spdlog::debug("Changed directory to root: {}", std::filesystem::current_path().string());

    if (!Config::instance().load()) {
        spdlog::error("Unable to load configuration file");
        return 1;
    }

    // raylib log level
    SetTraceLogLevel(LOG_WARNING);

    int width = Config::integer("window", "width");
    int height = Config::integer("window", "height");
    spdlog::info("Initializing window with resolution {}x{}", width, height);

    unsigned int flags = 0;
    if (Config::boolean("window", "fullscreen")) {
        flags |= FLAG_FULLSCREEN_MODE;
        spdlog::info("Enabling fullscreen");
    }

    if (Config::boolean("window", "vsync")) {
        flags |= FLAG_VSYNC_HINT;
        spdlog::info("Enabling vsync");
    }
    SetConfigFlags(flags);

    InitWindow(width, height, "weplay");

    spdlog::info("Initializing");

    {
        Console console;

        spdlog::info("Running");
        console.run();
    }

    spdlog::info("Exiting");
    CloseWindow();

    return 0;
}
