#pragma once

#ifdef CONSOLE_LOGGING
#include <spdlog/spdlog.h>
#else
namespace spdlog {
enum class level { debug };
void set_level(...);
void trace(...);
void debug(...);
void info(...);
void warn(...);
void error(...);
}
#endif
