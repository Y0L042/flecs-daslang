// Comprehensive tests for Flecs query, iteration, and world operation bindings.
options gen2
require flecs
require das_test

// ---------------------------------------------------------------------------
// World operations
// ---------------------------------------------------------------------------

def test_frame_lifecycle() {
    begin_case("ecs_frame_begin / ecs_frame_end complete without error")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    let dt = ecs_frame_begin(world, 0.016f)
    CHECK(dt >= 0.0f, "frame delta time is non-negative")
    ecs_frame_end(world)

    ecs_fini(world)
    end_case()
}

def test_multiple_frames() {
    begin_case("consecutive frame_begin / frame_end calls increment frame count")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    ecs_frame_begin(world, 0.016f)
    ecs_frame_end(world)
    ecs_frame_begin(world, 0.016f)
    ecs_frame_end(world)
    ecs_frame_begin(world, 0.016f)
    ecs_frame_end(world)

    let info = ecs_get_world_info(world)
    REQUIRE(info != null, "world_info non-null")
    CHECK(info.frame_count_total >= int64(3), "frame_count_total is at least 3 after 3 frames")

    ecs_fini(world)
    end_case()
}

def test_should_quit() {
    begin_case("ecs_should_quit is false before ecs_quit, true after")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    CHECK(!ecs_should_quit(world), "should_quit is false before ecs_quit")
    ecs_quit(world)
    CHECK(ecs_should_quit(world),  "should_quit is true after ecs_quit")

    ecs_fini(world)
    end_case()
}

def test_is_fini() {
    begin_case("ecs_is_fini returns false on a live world")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    CHECK(!ecs_is_fini(world), "ecs_is_fini is false on a live world")

    ecs_fini(world)
    end_case()
}

def test_get_build_info() {
    begin_case("ecs_get_build_info returns valid version info")
    let info = ecs_get_build_info()
    CHECK(info != null,            "ecs_get_build_info returns non-null")
    CHECK(int(info.version_major) >= 4, "Flecs major version is at least 4")
    end_case()
}

def test_get_world_info() {
    begin_case("ecs_get_world_info returns valid stats")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    ecs_new(world)
    ecs_new(world)

    let info = ecs_get_world_info(world)
    CHECK(info != null, "ecs_get_world_info returns non-null")

    ecs_fini(world)
    end_case()
}

def test_get_entities() {
    begin_case("ecs_get_entities reports alive entity count")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    ecs_new(world)
    ecs_new(world)

    let snap = ecs_get_entities(world)
    CHECK(snap.alive_count >= 2,          "alive_count is at least 2")
    CHECK(snap.count >= snap.alive_count, "total count >= alive_count")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Deferred operations
// ---------------------------------------------------------------------------

def test_defer_begin_end() {
    begin_case("ecs_defer_begin / ecs_defer_end toggle deferred mode")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    CHECK(!ecs_is_deferred(world), "not deferred before defer_begin")
    ecs_defer_begin(world)
    CHECK(ecs_is_deferred(world),  "deferred after defer_begin")
    ecs_defer_end(world)
    CHECK(!ecs_is_deferred(world), "not deferred after defer_end")

    ecs_fini(world)
    end_case()
}

def test_deferred_add() {
    begin_case("ecs_add_id inside defer is applied after ecs_defer_end")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    var e   = ecs_new(world)

    ecs_defer_begin(world)
    ecs_add_id(world, e, tag)
    ecs_defer_end(world)

    CHECK(ecs_has_id(world, e, tag), "deferred add_id is applied after defer_end")

    ecs_fini(world)
    end_case()
}

def test_deferred_delete() {
    begin_case("ecs_delete inside defer is applied after ecs_defer_end")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e = ecs_new(world)
    CHECK(ecs_is_alive(world, e), "entity alive before deferred delete")

    ecs_defer_begin(world)
    ecs_delete(world, e)
    ecs_defer_end(world)

    CHECK(!ecs_is_alive(world, e), "entity deleted after defer_end")

    ecs_fini(world)
    end_case()
}

def test_defer_suspend_resume() {
    begin_case("ecs_defer_suspend / ecs_defer_resume")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    ecs_defer_begin(world)
    CHECK(ecs_is_deferred(world), "deferred after begin")

    ecs_defer_suspend(world)
    CHECK(!ecs_is_deferred(world), "not deferred while suspended")

    ecs_defer_resume(world)
    CHECK(ecs_is_deferred(world),  "deferred again after resume")

    ecs_defer_end(world)
    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Iteration: ecs_each_id
// ---------------------------------------------------------------------------

def test_each_id_iteration() {
    begin_case("ecs_each_id / ecs_each_next iterates all entities with a tag")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    for (i in range(4)) {
        var e = ecs_new(world)
        ecs_add_id(world, e, tag)
    }
    ecs_new(world) // extra entity without tag

    var it    = ecs_each_id(world, tag)
    var count = 0
    unsafe {
        while (ecs_each_next(addr(it))) {
            count += it.count
        }
    }
    CHECK(count == 4, "ecs_each_id iterates exactly 4 matching entities")

    ecs_fini(world)
    end_case()
}

def test_each_id_empty() {
    begin_case("ecs_each_id on unused tag yields zero entities")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var unused_tag = ecs_new_low_id(world)

    var it    = ecs_each_id(world, unused_tag)
    var count = 0
    unsafe {
        while (ecs_each_next(addr(it))) {
            count += it.count
        }
    }
    CHECK(count == 0, "iterating an unused tag yields 0 entities")

    ecs_fini(world)
    end_case()
}

def test_each_id_after_remove() {
    begin_case("ecs_each_id excludes entities that had tag removed")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    var e1  = ecs_new(world)
    var e2  = ecs_new(world)
    var e3  = ecs_new(world)
    ecs_add_id(world, e1, tag)
    ecs_add_id(world, e2, tag)
    ecs_add_id(world, e3, tag)
    ecs_remove_id(world, e2, tag)

    var it    = ecs_each_id(world, tag)
    var count = 0
    unsafe {
        while (ecs_each_next(addr(it))) {
            count += it.count
        }
    }
    CHECK(count == 2, "only 2 entities remain after removing tag from one")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Iteration: ecs_children
// ---------------------------------------------------------------------------

def test_children_iteration() {
    begin_case("ecs_children / ecs_children_next iterates direct children")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    let EcsChildOf_id = ecs_lookup(world, "ChildOf")
    REQUIRE(EcsChildOf_id != uint64(0), "EcsChildOf built-in found")

    var parent = ecs_new(world)
    for (i in range(3)) {
        var child = ecs_new(world)
        ecs_add_id(world, child, ecs_make_pair(EcsChildOf_id, parent))
    }

    var it    = ecs_children(world, parent)
    var count = 0
    unsafe {
        while (ecs_children_next(addr(it))) {
            count += it.count
        }
    }
    CHECK(count == 3, "ecs_children finds exactly 3 children")

    ecs_fini(world)
    end_case()
}

def test_children_none() {
    begin_case("ecs_children on a leaf entity yields zero results")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var leaf = ecs_new(world)

    var it    = ecs_children(world, leaf)
    var count = 0
    unsafe {
        while (ecs_children_next(addr(it))) {
            count += it.count
        }
    }
    CHECK(count == 0, "leaf entity has no children")

    ecs_fini(world)
    end_case()
}

def test_nested_children() {
    begin_case("ecs_children only returns direct children, not grandchildren")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    let EcsChildOf_id = ecs_lookup(world, "ChildOf")
    REQUIRE(EcsChildOf_id != uint64(0), "EcsChildOf found")

    var grandparent = ecs_new(world)
    var parent      = ecs_new(world)
    var grandchild  = ecs_new(world)
    ecs_add_id(world, parent,     ecs_make_pair(EcsChildOf_id, grandparent))
    ecs_add_id(world, grandchild, ecs_make_pair(EcsChildOf_id, parent))

    var it    = ecs_children(world, grandparent)
    var count = 0
    unsafe {
        while (ecs_children_next(addr(it))) {
            count += it.count
        }
    }
    CHECK(count == 1, "grandparent has exactly 1 direct child")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Query lifecycle and iteration
// ---------------------------------------------------------------------------

def test_query_init_fini() {
    begin_case("ecs_query_init with expr / ecs_query_fini")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    ecs_set_name(world, tag, "QInitTag")

    var desc  : ecs_query_desc_t
    ecs_query_desc_set_expr(desc, "QInitTag")

    var query : ecs_query_t? = null
    unsafe {
        query = ecs_query_init(world, addr(desc))
    }
    REQUIRE(query != null, "ecs_query_init returns non-null")

    let qstr = ecs_query_str(query)
    CHECK(qstr != "", "ecs_query_str returns non-empty string")

    ecs_query_fini(query)
    ecs_fini(world)
    end_case()
}

def test_query_iteration() {
    begin_case("ecs_query_iter / ecs_query_next iterates matching entities")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    ecs_set_name(world, tag, "QIterTag")

    for (i in range(3)) {
        var e = ecs_new(world)
        ecs_add_id(world, e, tag)
    }
    ecs_new(world) // unmatched entity

    var desc  : ecs_query_desc_t
    ecs_query_desc_set_expr(desc, "QIterTag")

    var query : ecs_query_t? = null
    unsafe {
        query = ecs_query_init(world, addr(desc))
    }
    REQUIRE(query != null, "query_init succeeded")

    var it    = ecs_query_iter(world, query)
    var count = 0
    unsafe {
        while (ecs_query_next(addr(it))) {
            count += it.count
        }
    }
    CHECK(count == 3, "query finds exactly 3 matching entities")

    ecs_query_fini(query)
    ecs_fini(world)
    end_case()
}

def test_query_is_true() {
    begin_case("ecs_query_is_true returns true when matches exist, false when empty")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    ecs_set_name(world, tag, "QIsTrueTag")

    var desc  : ecs_query_desc_t
    ecs_query_desc_set_expr(desc, "QIsTrueTag")

    var query : ecs_query_t? = null
    unsafe {
        query = ecs_query_init(world, addr(desc))
    }
    REQUIRE(query != null, "query_init succeeded")

    CHECK(!ecs_query_is_true(query), "query_is_true is false when no entities match")

    var e = ecs_new(world)
    ecs_add_id(world, e, tag)
    CHECK(ecs_query_is_true(query), "query_is_true is true after adding matching entity")

    ecs_query_fini(query)
    ecs_fini(world)
    end_case()
}

def test_query_match_count() {
    begin_case("ecs_query_match_count returns number of matched tables")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    ecs_set_name(world, tag, "QMCTag")

    for (i in range(5)) {
        var e = ecs_new(world)
        ecs_add_id(world, e, tag)
    }

    var desc  : ecs_query_desc_t
    ecs_query_desc_set_expr(desc, "QMCTag")

    var query : ecs_query_t? = null
    unsafe {
        query = ecs_query_init(world, addr(desc))
    }
    REQUIRE(query != null, "query_init succeeded")

    let mc = ecs_query_match_count(query)
    CHECK(mc > 0, "match_count is positive when entities match")

    ecs_query_fini(query)
    ecs_fini(world)
    end_case()
}

def test_query_count() {
    begin_case("ecs_query_count returns entity and result counts")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    ecs_set_name(world, tag, "QCntTag")

    for (i in range(4)) {
        var e = ecs_new(world)
        ecs_add_id(world, e, tag)
    }

    var desc  : ecs_query_desc_t
    ecs_query_desc_set_expr(desc, "QCntTag")

    var query : ecs_query_t? = null
    unsafe {
        query = ecs_query_init(world, addr(desc))
    }
    REQUIRE(query != null, "query_init succeeded")

    var qc = ecs_query_count(query)
    CHECK(qc.results >= 1, "at least 1 result table")

    ecs_query_fini(query)
    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Iterator utilities
// ---------------------------------------------------------------------------

def test_iter_count() {
    begin_case("ecs_iter_count returns total matching entity count")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    for (i in range(3)) {
        var e = ecs_new(world)
        ecs_add_id(world, e, tag)
    }

    var it  = ecs_each_id(world, tag)
    var cnt = 0
    unsafe {
        cnt = ecs_iter_count(addr(it))
    }
    CHECK(cnt == 3, "ecs_iter_count returns 3 for 3 matching entities")

    ecs_fini(world)
    end_case()
}

def test_iter_is_true() {
    begin_case("ecs_iter_is_true returns true when matches exist")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var tag = ecs_new_low_id(world)
    for (i in range(2)) {
        var e = ecs_new(world)
        ecs_add_id(world, e, tag)
    }

    var it = ecs_each_id(world, tag)
    var is_true = false
    unsafe {
        is_true = ecs_iter_is_true(addr(it))
    }
    CHECK(is_true, "ecs_iter_is_true is true when entities match")

    ecs_fini(world)
    end_case()
}

def test_iter_is_true_empty() {
    begin_case("ecs_iter_is_true returns false when no entities match")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var empty_tag = ecs_new_low_id(world)

    var it = ecs_each_id(world, empty_tag)
    var is_true = true
    unsafe {
        is_true = ecs_iter_is_true(addr(it))
    }
    CHECK(!is_true, "ecs_iter_is_true is false for empty tag")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// ID utilities
// ---------------------------------------------------------------------------

def test_id_is_pair() {
    begin_case("ecs_id_is_pair correctly identifies pair vs plain ids")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var r    = ecs_new_low_id(world)
    var t    = ecs_new(world)
    let pair = ecs_make_pair(r, t)

    CHECK(ecs_id_is_pair(pair),  "pair id is a pair")
    CHECK(!ecs_id_is_pair(r),    "plain entity is not a pair")
    CHECK(!ecs_id_is_pair(t),    "target entity is not a pair")

    ecs_fini(world)
    end_case()
}

def test_id_is_valid() {
    begin_case("ecs_id_is_valid on live entity ids")
    var world = ecs_init()
    REQUIRE(world != null, "world init failed")

    var e    = ecs_new(world)
    let pair = ecs_make_pair(ecs_new_low_id(world), e)
    CHECK(ecs_id_is_valid(world, e),    "live entity id is valid")
    CHECK(ecs_id_is_valid(world, pair), "pair id is valid")

    ecs_fini(world)
    end_case()
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

[export]
def run_query_tests() {
    begin_suite("Flecs Query and Iterator API")
    // World operations
    test_frame_lifecycle()
    test_multiple_frames()
    test_should_quit()
    test_is_fini()
    test_get_build_info()
    test_get_world_info()
    test_get_entities()
    // Deferred operations
    test_defer_begin_end()
    test_deferred_add()
    test_deferred_delete()
    test_defer_suspend_resume()
    // each_id iteration
    test_each_id_iteration()
    test_each_id_empty()
    test_each_id_after_remove()
    // children iteration
    test_children_iteration()
    test_children_none()
    test_nested_children()
    // Query lifecycle and iteration
    test_query_init_fini()
    test_query_iteration()
    test_query_is_true()
    test_query_match_count()
    test_query_count()
    // Iterator utilities
    test_iter_count()
    test_iter_is_true()
    test_iter_is_true_empty()
    // ID utilities
    test_id_is_pair()
    test_id_is_valid()
    end_suite()
}
