#include "console.hpp"
#include <boost/log/trivial.hpp>

int main(int argc, char* argv[])
{
    BOOST_LOG_TRIVIAL(info) << "Initializing window";
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_FULLSCREEN_MODE);
    InitWindow(1920, 1080, "Raspberry Console");
    HideCursor();

    BOOST_LOG_TRIVIAL(info) << "Initializing raspberry console";
    Console console;

    BOOST_LOG_TRIVIAL(info) << "Running raspberry console";
    console.run();

    BOOST_LOG_TRIVIAL(info) << "Closing window";
    RlCloseWindow();

    return 0;
}
