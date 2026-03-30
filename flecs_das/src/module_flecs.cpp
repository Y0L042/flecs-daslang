#include "module_flecs.h"
#include <daScript/daScript.h>
#include <flecs.h>

MAKE_TYPE_FACTORY(ecs_world_t, ecs_world_t)
struct EcsWorldAnnotation : das::DummyTypeAnnotation
{
    EcsWorldAnnotation()
        : DummyTypeAnnotation("ecs_world_t", "ecs_world_t", sizeof(void*), alignof(void*)) {}
};

class Module_flecs : public das::Module
{
  public:
    Module_flecs() : Module("flecs")
    {
        das::ModuleLibrary lib(this);
        lib.addBuiltInModule();

        addAnnotation(das::make_smart<EcsWorldAnnotation>());

        das::addExtern<DAS_BIND_FUN(ecs_init)>(*this, lib, "ecs_init", das::SideEffects::modifyExternal, "ecs_init");
    }
};
REGISTER_MODULE(Module_flecs);
