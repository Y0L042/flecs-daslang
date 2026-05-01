/// @file
/// @brief Main binding module implementation for the flecs daScript bridge.
///
/// This file mixes handwritten glue code with generated registration fragments.
#include "module_flecs.h"
#include <daScript/daScript.h>

#include <flecs.h>
#include <flecs/addons/flecs_c.h>

#include "scripts/flecs.das.inc"
#include "scripts/flecs_c.das.inc"
#include "scripts/flecs_helpers.das.inc"

/// @name Hand-written helpers
/// Helpers in this section adapt Flecs APIs to the datatypes that daScript
/// exposes more naturally.
/// @{

/// Assign the query expression string on a query descriptor.
static void flecs_set_query_expr(ecs_query_desc_t &desc, const char *expr)
{
    desc.expr = expr;
}

/// Call ecs_set_id using ecs_size_t, which matches daScript typeinfo(sizeof ...).
static void flecs_set_id(ecs_world_t *world, ecs_entity_t entity, ecs_id_t id, ecs_size_t size, void *ptr)
{
    ecs_set_id(world, entity, id, (size_t)size, ptr);
}

/// Call ecs_field_w_size using ecs_size_t instead of size_t.
static void *flecs_field_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index)
{
    return ecs_field_w_size(it, (size_t)size, index);
}

/// Provide the self-owned field accessor shape expected by the Daslang layer.
static void *flecs_field_self_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index)
{
    return ecs_field_w_size(it, (size_t)size, index);
}

/// Call ecs_field_at_w_size using ecs_size_t instead of size_t.
static void *flecs_field_at_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index, int32_t row)
{
    return ecs_field_at_w_size(it, (size_t)size, index, row);
}

/// Set a term in ecs_query_desc_t by index.
/// Required because terms[] is a fixed C array, not directly indexable from Daslang.
static void flecs_query_desc_set_term(ecs_query_desc_t &desc, int32_t idx, ecs_id_t id, int32_t inout)
{
    desc.terms[idx].id = id;
    desc.terms[idx].inout = (ecs_inout_kind_t)inout;
}

/// Set a term with all three fields: id, inout, and oper.
/// Used by query builder methods that need to control the operator (e.g. without() uses EcsNot).
static void flecs_query_desc_set_term_full(ecs_query_desc_t &desc, int32_t idx, ecs_id_t id, int32_t inout, int32_t oper)
{
    desc.terms[idx].id = id;
    desc.terms[idx].inout = (ecs_inout_kind_t)inout;
    desc.terms[idx].oper = (ecs_oper_kind_t)oper;
}

/// Lookup a path using the Flecs path separator while keeping the prefix null.
static ecs_entity_t flecs_lookup_path(ecs_world_t *world, ecs_entity_t parent, const char *path, bool recursive)
{
    return ecs_lookup_path_w_sep(world, parent, path, "::", NULL, recursive);
}
/// Resolve a path from a parent entity with the Flecs path separator.
static char *flecs_get_path_from(ecs_world_t *world, ecs_entity_t parent, ecs_entity_t child)
{
    return ecs_get_path_w_sep(world, parent, child, "::", NULL);
}
/// Create an entity from a path using the Flecs path separator.
static ecs_entity_t flecs_new_from_path(ecs_world_t *world, ecs_entity_t parent, const char *path)
{
    return ecs_new_from_path_w_sep(world, parent, path, "::", NULL);
}
/// Add a path to an entity using the Flecs path separator.
static ecs_entity_t flecs_add_path(ecs_world_t *world, ecs_entity_t entity, ecs_entity_t parent, const char *path)
{
    return ecs_add_path_w_sep(world, entity, parent, path, "::", NULL);
}

/// Set cache_kind on ecs_query_desc_t.
static void flecs_query_desc_set_cache_kind(ecs_query_desc_t &desc, int32_t cache_kind)
{
    desc.cache_kind = (ecs_query_cache_kind_t)cache_kind;
}

/// @}

/// @name Hand-written annotations
/// These wrappers stay handwritten because the generator skips them.
/// @{

MAKE_TYPE_FACTORY(ecs_world_t, ecs_world_t);
/// Annotation for opaque ecs_world_t values in daScript.
struct EcsWorldAnnotation : das::DummyTypeAnnotation
{
    EcsWorldAnnotation() : DummyTypeAnnotation("ecs_world_t", "ecs_world_t", sizeof(void *), alignof(void *))
    {
    }
};

MAKE_TYPE_FACTORY(ecs_type_t, ecs_type_t);
/// Managed annotation for ecs_type_t, exposing array/count fields.
struct EcsTypeAnnotation : das::ManagedStructureAnnotation<ecs_type_t, false>
{
    EcsTypeAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_type_t", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(array)>("array", "array");
        addField<DAS_BIND_MANAGED_FIELD(count)>("count", "count");
    }
};

MAKE_TYPE_FACTORY(ecs_entities_t, ecs_entities_t);
/// Managed annotation for ecs_entities_t, exposing ids/count metadata.
struct EcsEntitesAnnotation : das::ManagedStructureAnnotation<ecs_entities_t, false>
{
    EcsEntitesAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_entities_t", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(ids)>("ids", "ids");
        addField<DAS_BIND_MANAGED_FIELD(count)>("count", "count");
        addField<DAS_BIND_MANAGED_FIELD(alive_count)>("alive_count", "alive_count");
    }
};

/// @}

/// Generated annotations are included verbatim from the binding generator.
#include "generated/module_flecs_annotations.inc"
#include "generated/module_flecs_constants.inc"

/// daScript module entry point for the flecs binding package.
class Module_flecs : public das::Module
{
    bool initialized = false;

  public:
    Module_flecs() : Module("flecs_core")
    {
    }

    bool initDependencies() override
    {
        if (initialized)
            return true;
        initialized = true;

        das::ModuleLibrary lib(this);
        lib.addBuiltInModule();

        // Hand-written registrations
        addAnnotation(new EcsWorldAnnotation());
        addAnnotation(new EcsTypeAnnotation(lib));
        addAnnotation(new EcsEntitesAnnotation(lib));

        das::addExtern<DAS_BIND_FUN(ecs_init)>(*this, lib, "ecs_init", das::SideEffects::modifyExternal, "ecs_init");
        das::addExtern<DAS_BIND_FUN(ecs_fini)>(*this, lib, "ecs_fini", das::SideEffects::modifyExternal, "ecs_fini");
        das::addExtern<DAS_BIND_FUN(ecs_get_entities), das::SimNode_ExtFuncCallAndCopyOrMove>(
            *this, lib, "ecs_get_entities", das::SideEffects::modifyExternal, "ecs_get_entities");

        // Generated registrations
#include "generated/module_flecs_register.inc"

        // Hand-written helpers
        das::addExtern<DAS_BIND_FUN(flecs_set_query_expr)>(*this, lib, "ecs_query_desc_set_expr",
                                                           das::SideEffects::modifyArgument, "flecs_set_query_expr");
        das::addExtern<DAS_BIND_FUN(flecs_set_id)>(*this, lib, "flecs_set_id", das::SideEffects::modifyExternal,
                                                   "flecs_set_id");
        das::addExtern<DAS_BIND_FUN(flecs_field_w_size)>(*this, lib, "flecs_field_w_size",
                                                         das::SideEffects::modifyExternal, "flecs_field_w_size");
        das::addExtern<DAS_BIND_FUN(flecs_field_self_w_size)>(
            *this, lib, "flecs_field_self_w_size", das::SideEffects::modifyExternal, "flecs_field_self_w_size");
        das::addExtern<DAS_BIND_FUN(flecs_field_at_w_size)>(*this, lib, "flecs_field_at_w_size",
                                                            das::SideEffects::modifyExternal, "flecs_field_at_w_size");
        das::addExtern<DAS_BIND_FUN(flecs_query_desc_set_term)>(
            *this, lib, "flecs_query_desc_set_term", das::SideEffects::modifyArgument, "flecs_query_desc_set_term");
        das::addExtern<DAS_BIND_FUN(flecs_query_desc_set_term_full)>(
            *this, lib, "flecs_query_desc_set_term_full", das::SideEffects::modifyArgument, "flecs_query_desc_set_term_full");
        das::addExtern<DAS_BIND_FUN(flecs_query_desc_set_cache_kind)>(*this, lib, "flecs_query_desc_set_cache_kind",
                                                                      das::SideEffects::modifyArgument,
                                                                      "flecs_query_desc_set_cache_kind");
        das::addExtern<DAS_BIND_FUN(flecs_lookup_path)>(*this, lib, "flecs_lookup_path", das::SideEffects::none,
                                                        "flecs_lookup_path");
        das::addExtern<DAS_BIND_FUN(flecs_get_path_from), das::SimNode_ExtFuncCallAndCopyOrMove>(
            *this, lib, "flecs_get_path_from", das::SideEffects::none, "flecs_get_path_from");
        das::addExtern<DAS_BIND_FUN(flecs_new_from_path)>(*this, lib, "flecs_new_from_path",
                                                          das::SideEffects::modifyExternal, "flecs_new_from_path");
        das::addExtern<DAS_BIND_FUN(flecs_add_path)>(*this, lib, "flecs_add_path", das::SideEffects::modifyExternal,
                                                     "flecs_add_path");

        compileBuiltinModule("flecs.das", flecs_das, sizeof(flecs_das));
        compileBuiltinModule("flecs_c.das", flecs_c_das, sizeof(flecs_c_das));
        // compileBuiltinModule("flecs_helpers.das", flecs_helpers_das, sizeof(flecs_helpers_das));

        return true;
    }
};
REGISTER_MODULE(Module_flecs);
