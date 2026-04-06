/// @file
/// @brief Public consumer header for registering the flecs daScript module.
///
/// Include this header from host applications that want to initialize the
/// `flecs` module and load the binding layer into daScript.
#pragma once

// Use NEED_MODULE(Module_flecs) (or PULL_MODULE) before das::Module::Initialize().

#include "module_flecs.h"
