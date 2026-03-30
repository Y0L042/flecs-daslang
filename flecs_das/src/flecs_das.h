#pragma once

// Public header for consumers of the flecs_das library.
// Include this to register the "flecs" daslang module, then call
// NEED_MODULE(FlecsModule) (or PULL_MODULE) before das::Module::Initialize().

#include "module_flecs.h"
