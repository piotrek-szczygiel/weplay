#include "console.hpp"
#include <boost/log/trivial.hpp>

int main(int argc, char* argv[])
{
    BOOST_LOG_TRIVIAL(info) << "Initializing raspberry console";
    Console console { 1280, 720 };

    BOOST_LOG_TRIVIAL(info) << "Running raspberry console";
    console.run();

    return 0;
}
