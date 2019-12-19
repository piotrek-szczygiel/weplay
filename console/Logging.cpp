#include "Logging.hpp"

#ifndef CONSOLE_LOGGING
namespace spdlog {
void set_level(...) {};
void trace(...) {};
void debug(...) {};
void info(...) {};
void warn(...) {};
void error(...) {};
}
#endif
