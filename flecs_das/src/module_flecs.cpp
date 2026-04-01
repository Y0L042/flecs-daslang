#include "module_flecs.h"
#include <daScript/daScript.h>

#include <flecs.h>
#include <flecs/addons/flecs_c.h>

#include "scripts/flecs.das.inc"
#include "scripts/flecs_c.das.inc"
#include "scripts/flecs_helpers.das.inc"

// --- Hand-written helpers ---

static void flecs_set_query_expr(ecs_query_desc_t &desc, const char *expr)
{
    desc.expr = expr;
}

// ecs_set_id with ecs_size_t (int) instead of size_t (uint64) — matches typeinfo(sizeof ...).
static void flecs_set_id(ecs_world_t *world, ecs_entity_t entity, ecs_id_t id, ecs_size_t size, void *ptr)
{
    ecs_set_id(world, entity, id, (size_t)size, ptr);
}

// ecs_field_w_size with ecs_size_t (int) instead of size_t (uint64).
static void *flecs_field_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index)
{
    return ecs_field_w_size(it, (size_t)size, index);
}

// ecs_field_self variant: ecs_field_self_w_size is not a real function in flecs —
// it only appears as the body of the ecs_field_self() macro. The actual declared
// function is ecs_field_w_size; the "self" semantics (assert field is owned) are
// a C macro concern and not needed here.
static void *flecs_field_self_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index)
{
    return ecs_field_w_size(it, (size_t)size, index);
}

// ecs_field_at_w_size with ecs_size_t (int) instead of size_t (uint64).
static void *flecs_field_at_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index, int32_t row)
{
    return ecs_field_at_w_size(it, (size_t)size, index, row);
}

// Set a term in ecs_query_desc_t by index.
// Required because terms[] is a fixed C array, not directly indexable from Daslang.
static void flecs_query_desc_set_term(ecs_query_desc_t &desc, int32_t idx, ecs_id_t id, int32_t inout)
{
    desc.terms[idx].id = id;
    desc.terms[idx].inout = (ecs_inout_kind_t)inout;
}

// Path helpers — the "prefix" param in the underlying flecs functions is semantically
// nullable (NULL = no prefix). Daslang binds all string args as non-nullable, so we
// provide wrappers that hard-code "::" separator and NULL prefix.
static ecs_entity_t flecs_lookup_path(ecs_world_t *world, ecs_entity_t parent, const char *path, bool recursive)
{
    return ecs_lookup_path_w_sep(world, parent, path, "::", NULL, recursive);
}
static char *flecs_get_path_from(ecs_world_t *world, ecs_entity_t parent, ecs_entity_t child)
{
    return ecs_get_path_w_sep(world, parent, child, "::", NULL);
}
static ecs_entity_t flecs_new_from_path(ecs_world_t *world, ecs_entity_t parent, const char *path)
{
    return ecs_new_from_path_w_sep(world, parent, path, "::", NULL);
}
static ecs_entity_t flecs_add_path(ecs_world_t *world, ecs_entity_t entity, ecs_entity_t parent, const char *path)
{
    return ecs_add_path_w_sep(world, entity, parent, path, "::", NULL);
}

// Set cache_kind on ecs_query_desc_t.
static void flecs_query_desc_set_cache_kind(ecs_query_desc_t &desc, int32_t cache_kind)
{
    desc.cache_kind = (ecs_query_cache_kind_t)cache_kind;
}

// --- Hand-written annotations (keep these; generator skips them) ---

MAKE_TYPE_FACTORY(ecs_world_t, ecs_world_t);
struct EcsWorldAnnotation : das::DummyTypeAnnotation
{
    EcsWorldAnnotation() : DummyTypeAnnotation("ecs_world_t", "ecs_world_t", sizeof(void *), alignof(void *))
    {
    }
};

MAKE_TYPE_FACTORY(ecs_type_t, ecs_type_t);
struct EcsTypeAnnotation : das::ManagedStructureAnnotation<ecs_type_t, false>
{
    EcsTypeAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_type_t", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(array)>("array", "array");
        addField<DAS_BIND_MANAGED_FIELD(count)>("count", "count");
    }
};

MAKE_TYPE_FACTORY(ecs_entities_t, ecs_entities_t);
struct EcsEntitesAnnotation : das::ManagedStructureAnnotation<ecs_entities_t, false>
{
    EcsEntitesAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_entities_t", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(ids)>("ids", "ids");
        addField<DAS_BIND_MANAGED_FIELD(count)>("count", "count");
        addField<DAS_BIND_MANAGED_FIELD(alive_count)>("alive_count", "alive_count");
    }
};

// --- Generated annotations ---
#include "generated/module_flecs_annotations.inc"

class Module_flecs : public das::Module
{
  public:
    Module_flecs() : Module("flecs")
    {
        das::ModuleLibrary lib(this);
        lib.addBuiltInModule();

        // Hand-written registrations
        addAnnotation(das::make_smart<EcsWorldAnnotation>());
        addAnnotation(das::make_smart<EcsTypeAnnotation>(lib));
        addAnnotation(das::make_smart<EcsEntitesAnnotation>(lib));

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
        compileBuiltinModule("flecs_helpers.das", flecs_helpers_das, sizeof(flecs_helpers_das));
    }
};
REGISTER_MODULE(Module_flecs);
