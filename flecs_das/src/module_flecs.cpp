#include "module_flecs.h"
#include <daScript/daScript.h>
#include <flecs.h>
#include "scripts/flecs.das.inc"

// --- Hand-written helpers ---

static void flecs_set_query_expr(ecs_query_desc_t &desc, const char *expr) {
    desc.expr = expr;
}

// ecs_set_id with ecs_size_t (int) instead of size_t (uint64) — matches typeinfo(sizeof ...).
static void flecs_set_id(
    ecs_world_t *world, ecs_entity_t entity, ecs_id_t id,
    ecs_size_t size, void *ptr)
{
    ecs_set_id(world, entity, id, (size_t)size, ptr);
}

// ecs_field_w_size with ecs_size_t (int) instead of size_t (uint64).
static void *flecs_field_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index) {
    return ecs_field_w_size(it, (size_t)size, index);
}

// Set a term in ecs_query_desc_t by index.
// Required because terms[] is a fixed C array, not directly indexable from Daslang.
static void flecs_query_desc_set_term(
    ecs_query_desc_t &desc, int32_t idx, ecs_id_t id, int32_t inout)
{
    desc.terms[idx].id = id;
    desc.terms[idx].inout = (ecs_inout_kind_t)inout;
}

// Set cache_kind on ecs_query_desc_t.
static void flecs_query_desc_set_cache_kind(ecs_query_desc_t &desc, int32_t cache_kind) {
    desc.cache_kind = (ecs_query_cache_kind_t)cache_kind;
}

// --- Hand-written annotations (keep these; generator skips them) ---

MAKE_TYPE_FACTORY(ecs_world_t, ecs_world_t);
struct EcsWorldAnnotation : das::DummyTypeAnnotation
{
    EcsWorldAnnotation()
        : DummyTypeAnnotation("ecs_world_t", "ecs_world_t", sizeof(void*), alignof(void*)) {}
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
        das::addExtern<DAS_BIND_FUN(flecs_set_id)>(*this, lib, "flecs_set_id",
            das::SideEffects::modifyExternal, "flecs_set_id");
        das::addExtern<DAS_BIND_FUN(flecs_field_w_size)>(*this, lib, "flecs_field_w_size",
            das::SideEffects::modifyExternal, "flecs_field_w_size");
        das::addExtern<DAS_BIND_FUN(flecs_query_desc_set_term)>(*this, lib, "flecs_query_desc_set_term",
            das::SideEffects::modifyArgument, "flecs_query_desc_set_term");
        das::addExtern<DAS_BIND_FUN(flecs_query_desc_set_cache_kind)>(*this, lib, "flecs_query_desc_set_cache_kind",
            das::SideEffects::modifyArgument, "flecs_query_desc_set_cache_kind");

        compileBuiltinModule("flecs.das", flecs_das, sizeof(flecs_das));
    }
};
REGISTER_MODULE(Module_flecs);
