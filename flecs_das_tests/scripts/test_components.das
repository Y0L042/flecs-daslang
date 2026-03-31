// Test component registration and entity-component interactions
// TODO: fill in cases once ecs_new, ecs_component_init, ecs_add_id,
//       ecs_has_id, ecs_remove_id, ecs_set_name, ecs_get_name, ecs_lookup
//       are bound in module_flecs.cpp.
options gen2
require flecs
require das_test

[export]
def run_component_tests() {
    begin_suite("Flecs Component and Entity API")

    begin_case("placeholder - component API not yet bound")
    MESSAGE("ecs_new, ecs_component_init, ecs_add_id etc. are not bound yet")
    MESSAGE("Add real CHECK calls here once the flecs module exposes these functions")
    end_case()

    end_suite()
}
