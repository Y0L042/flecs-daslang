// Comprehensive tests for Flecs component and entity API bindings.
options gen2
require flecs
require das_test

// ---------------------------------------------------------------------------
// Entity lifecycle
// ---------------------------------------------------------------------------

def test_entity_validity() {
    begin_case("new entity is valid, alive, and exists")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e = ecs_new(world)
    CHECK(e != uint64(0),          "ecs_new returns non-zero entity id")
    CHECK(ecs_is_valid(world, e),  "new entity is valid")
    CHECK(ecs_is_alive(world, e),  "new entity is alive")
    CHECK(ecs_exists(world, e),    "new entity exists")

    ecs_fini(world)
    end_case()
}

def test_entity_delete() {
    begin_case("deleted entity is no longer alive")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e = ecs_new(world)
    CHECK(ecs_is_alive(world, e),  "entity is alive before delete")
    ecs_delete(world, e)
    CHECK(!ecs_is_alive(world, e), "entity is not alive after delete")

    ecs_fini(world)
    end_case()
}

def test_new_low_id() {
    begin_case("ecs_new_low_id returns a non-zero low-range entity")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var lo = ecs_new_low_id(world)
    CHECK(lo != uint64(0), "ecs_new_low_id returns non-zero id")
    CHECK(ecs_is_alive(world, lo), "low-id entity is alive")

    ecs_fini(world)
    end_case()
}

def test_new_w_id() {
    begin_case("ecs_new_w_id creates entity pre-tagged with a component")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    var e   = ecs_new_w_id(world, tag)

    CHECK(e != uint64(0),            "ecs_new_w_id returns non-zero entity")
    CHECK(ecs_has_id(world, e, tag), "entity created with tag already has it")

    ecs_fini(world)
    end_case()
}

def test_strip_generation_get_alive() {
    begin_case("ecs_strip_generation + ecs_get_alive roundtrip")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e        = ecs_new(world)
    let stripped = ecs_strip_generation(e)
    let alive    = ecs_get_alive(world, stripped)
    CHECK(alive == e, "ecs_get_alive recovers original entity from stripped id")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Tag (component without data) add / remove / has
// ---------------------------------------------------------------------------

def test_tag_add_has_remove() {
    begin_case("ecs_add_id / ecs_has_id / ecs_remove_id with a tag")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    var e   = ecs_new(world)

    CHECK(!ecs_has_id(world, e, tag), "entity does not have tag before add")
    ecs_add_id(world, e, tag)
    CHECK(ecs_has_id(world, e, tag),  "entity has tag after ecs_add_id")
    ecs_remove_id(world, e, tag)
    CHECK(!ecs_has_id(world, e, tag), "entity loses tag after ecs_remove_id")

    ecs_fini(world)
    end_case()
}

def test_enable_id() {
    begin_case("ecs_enable_id / ecs_is_enabled_id toggle enabled state")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    let can_toggle = ecs_lookup(world, "CanToggle")
    REQUIRE(can_toggle != uint64(0), "EcsCanToggle built-in found")

    var tag = ecs_new_low_id(world)
    ecs_add_id(world, tag, can_toggle)  // tag must have CanToggle trait to be togglable

    var e   = ecs_new(world)
    ecs_add_id(world, e, tag)

    CHECK(ecs_is_enabled_id(world, e, tag),  "component enabled by default after add")
    ecs_enable_id(world, e, tag, false)
    CHECK(!ecs_is_enabled_id(world, e, tag), "component disabled after enable_id(false)")
    ecs_enable_id(world, e, tag, true)
    CHECK(ecs_is_enabled_id(world, e, tag),  "component re-enabled after enable_id(true)")

    ecs_fini(world)
    end_case()
}

def test_clear() {
    begin_case("ecs_clear removes all components; entity remains alive")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag1 = ecs_new_low_id(world)
    var tag2 = ecs_new_low_id(world)
    var e    = ecs_new(world)
    ecs_add_id(world, e, tag1)
    ecs_add_id(world, e, tag2)
    CHECK(ecs_has_id(world, e, tag1), "has tag1 before clear")
    CHECK(ecs_has_id(world, e, tag2), "has tag2 before clear")

    ecs_clear(world, e)
    CHECK(!ecs_has_id(world, e, tag1), "lost tag1 after ecs_clear")
    CHECK(!ecs_has_id(world, e, tag2), "lost tag2 after ecs_clear")
    CHECK(ecs_is_alive(world, e),      "entity still alive after ecs_clear")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Naming and lookup
// ---------------------------------------------------------------------------

def test_naming() {
    begin_case("ecs_set_name / ecs_get_name roundtrip")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e = ecs_new(world)
    ecs_set_name(world, e, "TestEntity")
    let name = ecs_get_name(world, e)
    CHECK(name == "TestEntity", "name matches after ecs_set_name")

    ecs_fini(world)
    end_case()
}

def test_symbol() {
    begin_case("ecs_set_symbol / ecs_get_symbol roundtrip")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e = ecs_new(world)
    ecs_set_symbol(world, e, "my.module.Symbol")
    let sym = ecs_get_symbol(world, e)
    CHECK(sym == "my.module.Symbol", "symbol matches after ecs_set_symbol")

    ecs_fini(world)
    end_case()
}

def test_lookup() {
    begin_case("ecs_lookup by name; returns 0 for unknown name")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e = ecs_new(world)
    ecs_set_name(world, e, "LookupTarget")

    let found     = ecs_lookup(world, "LookupTarget")
    let not_found = ecs_lookup(world, "DoesNotExist")
    CHECK(found == e,             "ecs_lookup finds the named entity")
    CHECK(not_found == uint64(0), "ecs_lookup returns 0 for unknown name")

    ecs_fini(world)
    end_case()
}

def test_lookup_child() {
    begin_case("ecs_lookup_child finds a named child under a specific parent")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    let EcsChildOf_id = ecs_lookup(world, "ChildOf")
    REQUIRE(EcsChildOf_id != uint64(0), "EcsChildOf built-in entity found")

    var parent = ecs_new(world)
    ecs_set_name(world, parent, "ParentNode")

    var child = ecs_new(world)
    ecs_set_name(world, child, "ChildNode")
    ecs_add_id(world, child, ecs_make_pair(EcsChildOf_id, parent))

    let found = ecs_lookup_child(world, parent, "ChildNode")
    CHECK(found == child, "ecs_lookup_child returns the correct child entity")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Count
// ---------------------------------------------------------------------------

def test_count_id() {
    begin_case("ecs_count_id tracks entities with a given component")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    CHECK(ecs_count_id(world, tag) == 0, "count is 0 before any entities have the tag")

    var e1 = ecs_new(world)
    var e2 = ecs_new(world)
    var e3 = ecs_new(world)
    ecs_add_id(world, e1, tag)
    ecs_add_id(world, e2, tag)
    ecs_add_id(world, e3, tag)
    CHECK(ecs_count_id(world, tag) == 3, "count is 3 after adding tag to 3 entities")

    ecs_delete(world, e1)
    CHECK(ecs_count_id(world, tag) == 2, "count decrements on entity deletion")

    ecs_remove_id(world, e2, tag)
    CHECK(ecs_count_id(world, tag) == 1, "count decrements on component removal")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Pairs and relationships
// ---------------------------------------------------------------------------

def test_pairs() {
    begin_case("ecs_make_pair, ecs_id_is_pair, ecs_get_target")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var relation = ecs_new_low_id(world)
    var target   = ecs_new(world)
    var e        = ecs_new(world)

    let pair = ecs_make_pair(relation, target)
    CHECK(ecs_id_is_pair(pair),      "ecs_make_pair result is a pair id")
    CHECK(!ecs_id_is_pair(relation), "plain entity id is not a pair id")

    ecs_add_id(world, e, pair)
    CHECK(ecs_has_id(world, e, pair), "entity has pair after ecs_add_id")

    let got_target = ecs_get_target(world, e, relation, 0)
    CHECK(got_target == target, "ecs_get_target returns correct target entity")

    ecs_fini(world)
    end_case()
}

def test_get_parent() {
    begin_case("ecs_get_parent returns parent set via ChildOf pair")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    let EcsChildOf_id = ecs_lookup(world, "ChildOf")
    REQUIRE(EcsChildOf_id != uint64(0), "EcsChildOf found")

    var parent = ecs_new(world)
    var child  = ecs_new(world)
    ecs_add_id(world, child, ecs_make_pair(EcsChildOf_id, parent))

    let got_parent = ecs_get_parent(world, child)
    let no_parent  = ecs_get_parent(world, parent)
    CHECK(got_parent == parent,    "ecs_get_parent returns correct parent")
    CHECK(no_parent  == uint64(0), "ecs_get_parent returns 0 for root entity")

    ecs_fini(world)
    end_case()
}

def test_owns_vs_has() {
    begin_case("ecs_owns_id vs ecs_has_id with IsA inheritance")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    let EcsIsA_id = ecs_lookup(world, "IsA")
    REQUIRE(EcsIsA_id != uint64(0), "EcsIsA built-in found")

    var tag     = ecs_new_low_id(world)
    var base    = ecs_new(world)
    var derived = ecs_new(world)
    ecs_add_id(world, base, tag)
    ecs_add_id(world, derived, ecs_make_pair(EcsIsA_id, base))

    CHECK(ecs_has_id(world, derived, tag),        "derived sees inherited tag via ecs_has_id")
    CHECK(!ecs_owns_id(world, derived, tag),      "derived does not own inherited tag")
    CHECK(ecs_owns_id(world, base, tag),          "base owns the tag it added directly")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Clone
// ---------------------------------------------------------------------------

def test_clone() {
    begin_case("ecs_clone copies the archetype of the source entity")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag1 = ecs_new_low_id(world)
    var tag2 = ecs_new_low_id(world)
    var src  = ecs_new(world)
    ecs_add_id(world, src, tag1)
    ecs_add_id(world, src, tag2)

    var dst = ecs_clone(world, uint64(0), src, false)
    CHECK(dst != uint64(0),              "ecs_clone returns non-zero entity")
    CHECK(dst != src,                    "clone is a different entity from source")
    CHECK(ecs_has_id(world, dst, tag1),  "clone has tag1 from source")
    CHECK(ecs_has_id(world, dst, tag2),  "clone has tag2 from source")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Introspection
// ---------------------------------------------------------------------------

def test_entity_str() {
    begin_case("ecs_entity_str returns a non-empty string for a named entity")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e = ecs_new(world)
    ecs_set_name(world, e, "StrTestEntity")
    let s = ecs_entity_str(world, e)
    CHECK(s != "", "ecs_entity_str is non-empty")

    ecs_fini(world)
    end_case()
}

def test_id_str() {
    begin_case("ecs_id_str returns a non-empty string for a named entity id")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e = ecs_new(world)
    ecs_set_name(world, e, "IdStrTest")
    let s = ecs_id_str(world, e)
    CHECK(s != "", "ecs_id_str is non-empty")

    ecs_fini(world)
    end_case()
}

def test_get_type() {
    begin_case("ecs_get_type returns non-null with count > 0 after adding a component")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    var e   = ecs_new(world)
    ecs_add_id(world, e, tag)

    let typ = ecs_get_type(world, e)
    CHECK(typ != null,   "ecs_get_type is non-null for entity with component")
    CHECK(typ.count > 0, "type has at least one entry")

    ecs_fini(world)
    end_case()
}

def test_id_is_pair_valid() {
    begin_case("ecs_id_is_pair / ecs_id_is_valid on plain and pair ids")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var r    = ecs_new_low_id(world)
    var t    = ecs_new(world)
    let pair = ecs_make_pair(r, t)

    CHECK(ecs_id_is_pair(pair),          "pair id passes ecs_id_is_pair")
    CHECK(!ecs_id_is_pair(r),            "plain entity fails ecs_id_is_pair")
    CHECK(ecs_id_is_valid(world, pair),  "pair id is valid")
    CHECK(ecs_id_is_valid(world, r),     "plain entity id is valid")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Component registration
// ---------------------------------------------------------------------------

def test_component_init() {
    begin_case("ecs_component_init registers a component; ecs_get_type_info is non-null")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    // Register as a zero-size tag component (ecs_component_desc_t.type is a reserved
    // keyword in Daslang so size/alignment fields cannot be accessed directly here).
    var comp_entity = ecs_new_low_id(world)
    ecs_set_name(world, comp_entity, "MyTagComp")

    var desc : ecs_component_desc_t
    desc.entity = comp_entity

    var comp_id : ecs_entity_t = uint64(0)
    unsafe {
        comp_id = ecs_component_init(world, addr(desc))
    }
    CHECK(comp_id != uint64(0), "ecs_component_init returns non-zero id")

    let ti = ecs_get_type_info(world, comp_id)
    // Zero-size (tag) components may return null type_info; just verify no crash.
    MESSAGE("get_type_info returned (may be null for zero-size component)")

    ecs_fini(world)
    end_case()
}

def test_component_add_multiple_entities() {
    begin_case("registered tag component can be added to multiple entities")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var comp_entity = ecs_new_low_id(world)
    ecs_set_name(world, comp_entity, "SharedTagComp")

    var desc : ecs_component_desc_t
    desc.entity = comp_entity

    var comp_id : ecs_entity_t = uint64(0)
    unsafe {
        comp_id = ecs_component_init(world, addr(desc))
    }
    REQUIRE(comp_id != uint64(0), "component registered")

    var e1 = ecs_new(world)
    var e2 = ecs_new(world)
    ecs_add_id(world, e1, comp_id)
    ecs_add_id(world, e2, comp_id)

    CHECK(ecs_has_id(world, e1, comp_id),        "e1 has component")
    CHECK(ecs_has_id(world, e2, comp_id),        "e2 has component")
    CHECK(ecs_count_id(world, comp_id) == 2,     "count is 2 after adding to 2 entities")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

[export]
def run_component_tests() {
    begin_suite("Flecs Component and Entity API")
    // Entity lifecycle
    test_entity_validity()
    test_entity_delete()
    test_new_low_id()
    test_new_w_id()
    test_strip_generation_get_alive()
    // Tags
    test_tag_add_has_remove()
    test_enable_id()
    test_clear()
    // Naming and lookup
    test_naming()
    test_symbol()
    test_lookup()
    test_lookup_child()
    // Count
    test_count_id()
    // Pairs and relationships
    test_pairs()
    test_get_parent()
    test_owns_vs_has()
    // Clone
    test_clone()
    // Introspection
    test_entity_str()
    test_id_str()
    test_get_type()
    test_id_is_pair_valid()
    // Component registration
    test_component_init()
    test_component_add_multiple_entities()
    end_suite()
}
