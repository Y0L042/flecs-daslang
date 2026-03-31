// Test query API and iteration
// TODO: fill in cases once ecs_query_init, ecs_query_iter, ecs_query_next,
//       ecs_field_w_size, ecs_iter_fini, ecs_query_fini, ecs_frame_begin,
//       ecs_frame_end are bound in module_flecs.cpp.
options gen2
require flecs
require das_test

[export]
def run_query_tests() {
    begin_suite("Flecs Query and Iterator API")

    begin_case("placeholder - query API not yet bound")
    MESSAGE("ecs_query_init, ecs_query_iter, ecs_frame_begin etc. are not bound yet")
    MESSAGE("Add real CHECK calls here once the flecs module exposes these functions")
    end_case()

    end_suite()
}
