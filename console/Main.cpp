#include "Console.hpp"
#include <boost/log/trivial.hpp>

int main(int argc, char* argv[])
{
    BOOST_LOG_TRIVIAL(info) << "Current working directory: " << GetWorkingDirectory();

    BOOST_LOG_TRIVIAL(info) << "Initializing window";
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT);
    InitWindow(3000, 2000, "Raspberry Console");
    HideCursor();

    BOOST_LOG_TRIVIAL(info) << "Initializing raspberry console";
    Console console;

    BOOST_LOG_TRIVIAL(info) << "Running raspberry console";
    console.run();

    BOOST_LOG_TRIVIAL(info) << "Closing window";
    RlCloseWindow();

    return 0;
}
