#include "Util.hpp"

thread_local std::mt19937 gen { std::random_device {}() }; // NOLINT