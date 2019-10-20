#include "Controller.hpp"
#include "TcpServer.hpp"
#include <boost/log/trivial.hpp>

void Controller::worker()
{
    BOOST_LOG_TRIVIAL(info) << "Starting controller worker";

    try {
        TcpServer server { ctx_, 1984, state };
        ctx_.run();
    } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Exception while running TCP server: " << e.what();
    }
}
