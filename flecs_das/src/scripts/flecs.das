/// @file
/// @brief Base Daslang aliases and constants for the flecs binding layer.
///
/// The handwritten declarations in this file define the shared primitive types
/// and enums used by the higher-level wrappers.
options gen2

/// Shared Flecs identifier aliases used throughout the binding surface.
typedef ecs_id_t		= uint64;
typedef ecs_entity_t	= uint64;

/// Generated bindings follow. They are regenerated from Flecs metadata.

// BEGIN GENERATED (gen_flecs_bindings.py) - do not edit below this line
typedef ecs_flags8_t = uint8;
typedef ecs_flags16_t = uint16;
typedef ecs_flags32_t = uint;
typedef ecs_flags64_t = uint64;
typedef ecs_size_t = int;
typedef ecs_float_t = float;
typedef ecs_ftime_t = float;
typedef ecs_termset_t = uint;

// Enum constants
// ecs_inout_kind_t
let EcsInOutDefault : int = 0
let EcsInOutNone : int = 1
let EcsInOutFilter : int = 2
let EcsInOut : int = 3
let EcsIn : int = 4
let EcsOut : int = 5
// ecs_oper_kind_t
let EcsAnd : int = 0
let EcsOr : int = 1
let EcsNot : int = 2
let EcsOptional : int = 3
let EcsAndFrom : int = 4
let EcsOrFrom : int = 5
let EcsNotFrom : int = 6
// ecs_query_cache_kind_t
let EcsQueryCacheDefault : int = 0
let EcsQueryCacheAuto : int = 1
let EcsQueryCacheAll : int = 2
let EcsQueryCacheNone : int = 3
