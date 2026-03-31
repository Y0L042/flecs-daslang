// Test basic world creation and lifecycle
options gen2
require flecs
require das_test

def test_world_creation() {
    begin_case("ecs_init returns non-null world")
    var world : ecs_world_t? = ecs_init()
    CHECK(world != null, "ecs_init() should return a non-null world pointer")
    REQUIRE(world != null, "cannot proceed with ecs_fini on a null world")
    end_case()

    begin_case("ecs_fini returns 0")
    var res : int = ecs_fini(world)
    CHECK(res == 0, "ecs_fini() should return 0 on success")
    end_case()
}

def test_entity_creation() {
    begin_case("ecs_new returns valid entity")
    var world : ecs_world_t? = ecs_init()
    REQUIRE(world != null, "ecs_init() should return a non-null world pointer")

    var entity : ecs_entity_t = ecs_new(world)
    CHECK(entity != uint64(0), "ecs_new() should return a non-zero entity ID")

    ecs_fini(world)
    end_case()
}

[export]
def run_tests() {
    begin_suite("Flecs World Lifecycle")
    test_world_creation()
    test_entity_creation()
    end_suite()
}
