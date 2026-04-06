/// @file
/// @brief Public header for the flecs daScript module implementation.
///
/// The implementation lives in module_flecs.cpp. Consumers should prefer
/// flecs_das.h unless they are extending the module itself.
#pragma once

// flecs C API - single-header, static build
#ifndef flecs_STATIC
#define flecs_STATIC
#endif /* flecs_STATIC */

#include <flecs.h>