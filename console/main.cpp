#include "console.hpp"
#include <boost/log/trivial.hpp>

int main(int argc, char* argv[])
{
    BOOST_LOG_TRIVIAL(info) << "Initializing raspberry console";
    Console console;

    console.init();
    console.run();

    return 0;
}