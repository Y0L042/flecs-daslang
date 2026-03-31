#include "module_flecs.h"
#include <daScript/daScript.h>
#include <flecs.h>
#include "scripts/flecs.das.inc"

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

        compileBuiltinModule("flecs.das", flecs_das, sizeof(flecs_das));
    }
};
REGISTER_MODULE(Module_flecs);
