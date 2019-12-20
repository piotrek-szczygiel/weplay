#pragma once

// This fixes build errors on Windows caused by name collisions

#ifdef _WIN32
#define NOGDI
#define NOUSER
#define MMNOSOUND

#include <winsock2.h>

#include <windows.h>

#include "Logging.hpp"
#include <iphlpapi.h>
#endif
