#include "Console.hpp"
#include <boost/log/trivial.hpp>

int main(int argc, char* argv[])
{
    BOOST_LOG_TRIVIAL(info) << "Current working directory: " << GetWorkingDirectory();

    BOOST_LOG_TRIVIAL(info) << "Initializing temporary window";
    InitWindow(640, 480, "Temporary window");
    int width = GetMonitorWidth(0);
    int height = GetMonitorHeight(0);
    RlCloseWindow();

    BOOST_LOG_TRIVIAL(info) << "Initializing window with resolution " << width << "x" << height;
    // SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(width, height, "Raspberry Console");

    BOOST_LOG_TRIVIAL(info) << "Initializing raspberry console";
    Console console;

    BOOST_LOG_TRIVIAL(info) << "Running raspberry console";
    console.run();

    BOOST_LOG_TRIVIAL(info) << "Closing window";
    RlCloseWindow();

    return 0;
}
