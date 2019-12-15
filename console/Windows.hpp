#pragma once

// This fixes build errors on Windows caused by name collisions

#ifdef _WIN32
#define NOGDI
#define NOUSER

#include <spdlog/spdlog.h>
#endif
