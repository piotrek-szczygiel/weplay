#include "console.hpp"
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP; // NOLINT

int main(int argc, char* argv[])
{
    START_EASYLOGGINGPP(argc, argv);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

    Console console;

    console.init();
    console.run();

    return 0;
}