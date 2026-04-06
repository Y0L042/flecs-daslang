options gen2
require flecs

/**
 * @file sandbox.das
 * @brief Interactive sandbox script for validating flecs_das bindings.
 *
 * This script is not a strict pass/fail test suite. It is a staged playground
 * that demonstrates equivalent ways to build worlds, register components,
 * compose queries, and iterate results through different API layers.
 *
 * Layers:
 * - layer_1: raw C-level bindings and explicit descriptors.
 * - layer_2: helper wrappers that mimic common Flecs C macro workflows.
 * - layer_2b: query expression string variant and field access helpers.
 */



/*
#include <basics.h>
#include <stdio.h>

typedef struct {
    double x;
    double y;
} Position, Velocity;

int main(int argc, char *argv[]) {
    ecs_world_t *ecs = ecs_init_w_args(argc, argv);

    ECS_COMPONENT(ecs, Position);
    ECS_COMPONENT(ecs, Velocity);

    // Create a cached query for Position, Velocity. Cached queries are the 
    // fastest way to iterate entities as they cache results.
    ecs_query_t *q = ecs_query(ecs, {
        .terms = {
            { .id = ecs_id(Position) }, 
            { .id = ecs_id(Velocity), .inout = EcsIn}
        },

        // QueryCache Auto automatically caches all terms that can be cached.
        .cache_kind = EcsQueryCacheAuto
    });

    // Create a few test entities for a Position, Velocity query
    ecs_entity_t e1 = ecs_entity(ecs, { .name = "e1" });
    ecs_set(ecs, e1, Position, {10, 20});
    ecs_set(ecs, e1, Velocity, {1, 2});

    ecs_entity_t e2 = ecs_entity(ecs, { .name = "e2" });
    ecs_set(ecs, e2, Position, {10, 20});
    ecs_set(ecs, e2, Velocity, {3, 4});

    // This entity will not match as it does not have Position, Velocity
    ecs_entity_t e3 = ecs_entity(ecs, { .name = "e3" });
    ecs_set(ecs, e3, Position, {10, 20});

    // Iterate entities matching the query
    ecs_iter_t it = ecs_query_iter(ecs, q);

    // Outer loop, iterates archetypes
    while (ecs_query_next(&it)) {
        Position *p = ecs_field(&it, Position, 0);
        const Velocity *v = ecs_field(&it, Velocity, 1);

        // Inner loop, iterates entities in archetype
        for (int i = 0; i < it.count; i ++) {
            p[i].x += v[i].x;
            p[i].y += v[i].y;
            printf("%s: {%f, %f}\n", ecs_get_name(ecs, it.entities[i]), 
                p[i].x, p[i].y);
        }
    }

    // Cleanup query resources.
    ecs_query_fini(q);

    return ecs_fini(ecs);
}
*/

/*

#define ecs_query(world, ...)\
    ecs_query_init(world, &(ecs_query_desc_t) __VA_ARGS__ )

ecs_query_t* ecs_query_init(
    ecs_world_t *world,
    const ecs_query_desc_t *desc);

ecs_query_desc_t q_desc = {
    .terms = {
        { .id = ecs_id(Position) }, 
        { .id = ecs_id(Velocity), .inout = EcsIn}
    },
    .cache_kind = EcsQueryCacheAuto
};

OR

ecs_term_t term_pos;
term_pos.id = ecs_id(Position);

ecs_term_t term_vel;
term_vel.id = ecs_id(Velocity);
term_vel.inout = EcsIn;

ecs_term_t terms[] = { term_pos, term_vel };

ecs_query_cache_kind_t cache_kind = EcsQueryCacheAuto;

ecs_query_desc_t q_desc;
q_desc.terms = terms;
q_desc.cache_kind = cache_kind;


let term_pos = ecs_term_t(id=ecs_id(Position))
let term_vel = ecs_term_t(id=ecs_id(Velocity), inout=EcsIn)
let q_desc = ecs_query_desc_t(terms=[term_pos, term_vel], cache_kind=EcsQueryCacheAuto)

*/



/* ======== TESTING FOR BETER HELPER FUNCTIONS/WRAPPERS ======== */



/* ============================================================= */








[export]
/// @brief Runs all sandbox layers in sequence and prints sample output.
def sandbox() : void {
    print("=== Flecs Sandbox ===\n")
    print("--- Layer 1: Basic world setup, component registration, query, and iteration ---\n")
    layer_1()
    print("\n--- Layer 2: Wrappers for flecs_c.h convenience macros ---\n")
    layer_2()
    print("\n--- Layer 2b: Same as Layer 2, but with string expressions for queries, iterators for components ---\n")
    layer_2b()
    print("\n=== End of Sandbox ===")
}



struct Position {
    x : double
    y : double
}

struct Velocity {
    x : double
    y : double
}



// Using C functions directly, without any defines or helpers, to test the raw bindings and unsafe blocks.
/// @brief Raw bindings path using explicit component/query descriptors.
def layer_1() : void {
    var world : ecs_world_t? = ecs_init()

    // --- Register Position component (ECS_COMPONENT equivalent) ---
    let pos_edesc = ecs_entity_desc_t(name="Position", use_low_id=true)
    var pos_entity : ecs_entity_t
    unsafe { 
        pos_entity = ecs_entity_init(world, addr(pos_edesc)) 
    }

    let pos_cdesc = ecs_component_desc_t(
        entity = pos_entity,
        vtype = ecs_type_info_t(size=typeinfo sizeof(type<Position>), 
        alignment=typeinfo alignof(type<Position>))
    )

    var pos_id : ecs_entity_t
    unsafe { 
        pos_id = ecs_component_init(world, addr(pos_cdesc)) 
    }

    // --- Register Velocity component (ECS_COMPONENT equivalent) ---
    let vel_edesc = ecs_entity_desc_t(name="Velocity", use_low_id=true)
    var vel_entity : ecs_entity_t
    unsafe { 
        vel_entity = ecs_entity_init(world, addr(vel_edesc)) 
    }

    var vel_cdesc : ecs_component_desc_t
    vel_cdesc.entity = vel_entity
    vel_cdesc.vtype.size      = typeinfo sizeof(type<Velocity>)
    vel_cdesc.vtype.alignment = typeinfo alignof(type<Velocity>)
    var vel_id : ecs_entity_t
    unsafe { 
        vel_id = ecs_component_init(world, addr(vel_cdesc)) 
    }

    // --- Build query: Position (rw), Velocity (read-only), auto-cached ---
    var q_desc : ecs_query_desc_t
    flecs_query_desc_set_term(q_desc, 0, pos_id, EcsInOutDefault)
    flecs_query_desc_set_term(q_desc, 1, vel_id, EcsIn)
    flecs_query_desc_set_cache_kind(q_desc, EcsQueryCacheAuto)
    var q : ecs_query_t?
    unsafe { 
        q = ecs_query_init(world, addr(q_desc)) 
    }

    // --- e1: Position {10,20} + Velocity {1,2} ---
    let e1_desc = ecs_entity_desc_t(name="e1")
    var e1 : ecs_entity_t
    unsafe { 
        e1 = ecs_entity_init(world, addr(e1_desc)) 
    }
    var e1_pos = Position(x=10.0d, y=20.0d)
    var e1_vel = Velocity(x=1.0d, y=2.0d)
    unsafe {
        flecs_set_id(world, e1, pos_id, typeinfo sizeof(type<Position>), addr(e1_pos))
        flecs_set_id(world, e1, vel_id, typeinfo sizeof(type<Velocity>), addr(e1_vel))
    }

    // --- e2: Position {10,20} + Velocity {3,4} ---
    let e2_desc = ecs_entity_desc_t(name="e2")
    var e2 : ecs_entity_t
    unsafe { 
        e2 = ecs_entity_init(world, addr(e2_desc)) 
    }
    var e2_pos = Position(x=10.0d, y=20.0d)
    var e2_vel = Velocity(x=3.0d, y=4.0d)
    unsafe {
        flecs_set_id(world, e2, pos_id, typeinfo sizeof(type<Position>), addr(e2_pos))
        flecs_set_id(world, e2, vel_id, typeinfo sizeof(type<Velocity>), addr(e2_vel))
    }

    // --- e3: Position only — will not match the query ---
    let e3_desc = ecs_entity_desc_t(name="e3")
    var e3 : ecs_entity_t
    unsafe { 
        e3 = ecs_entity_init(world, addr(e3_desc)) 
    }
    var e3_pos = Position(x=10.0d, y=20.0d)
    unsafe { 
        flecs_set_id(world, e3, pos_id, typeinfo sizeof(type<Position>), addr(e3_pos)) 
    }

    // --- Iterate: outer loop over archetypes, inner over entities ---
    var it = ecs_query_iter(world, q)
    unsafe {
        while (ecs_query_next(addr(it))) {
            // ecs_field(&it, Position, 0) / ecs_field(&it, Velocity, 1)
            var p = reinterpret<Position?>(flecs_field_w_size(addr(it), typeinfo sizeof(type<Position>), int8(0)))
            var v = reinterpret<Velocity?>(flecs_field_w_size(addr(it), typeinfo sizeof(type<Velocity>), int8(1)))
            for (i in range(it.count)) {
                p[i].x += v[i].x
                p[i].y += v[i].y
                let name = ecs_get_name(world, it.entities[i])
                print("{name}: \{\{ {p[i].x}, {p[i].y} \}\}\n")
            }
        }
    }

    ecs_query_fini(q)
    ecs_fini(world)
}



// Using Layer 2 wrappers (flecs_c.das) to mimic typical flecs C macro usage.
/// @brief Wrapper-driven path with typed helper utilities.
def layer_2() : void {
    var world : ecs_world_t? = ecs_init()

    // --- ECS_COMPONENT equivalent ---
    let pos_id = ecs_component_register(world, type<Position>)
    let vel_id = ecs_component_register(world, type<Velocity>)

    // --- Build query via expression string ---
    // let q = ecs_query_str(world, "Position, [in] Velocity") // another option, using string expressions for queries. Note: this is more expensive at runtime due to parsing, but can be more concise and readable for complex queries.
    let q = ecs_query_builder(
        world, 
        [
            ecs_term(world, type<Position>),
            ecs_term(world, type<Velocity>, EcsIn)
        ], 
        EcsQueryCacheAuto
    )

    // --- Create entities with ecs_entity_new + ecs_set ---
    let e1 = ecs_entity_new(world, "e1")
    ecs_set(world, e1, Position(x=10.0d, y=20.0d))
    ecs_set(world, e1, Velocity(x=1.0d, y=2.0d))

    let e2 = ecs_entity_new(world, "e2")
    ecs_set(world, e2, Position(x=10.0d, y=20.0d))
    ecs_set(world, e2, Velocity(x=3.0d, y=4.0d))

    // e3: Position only — will not match the query
    let e3 = ecs_entity_new(world, "e3")
    ecs_set(world, e3, Position(x=10.0d, y=20.0d))

    // --- Iterate using ecs_field ---
    var it = ecs_query_iter(world, q)
    unsafe {
        while (ecs_query_next_iter(it)) {
            var p : Position? = ecs_field_get(it, type<Position>, 0)
            var v : Velocity? = ecs_field_get(it, type<Velocity>, 1)
            for (i in range(it.count)) {
                p[i].x += v[i].x
                p[i].y += v[i].y
                let name = ecs_get_name(world, it.entities[i])
                print("{name}: \{\{ {p[i].x}, {p[i].y} \}\}\n")
            }
        }
    }

    ecs_query_fini(q)
    ecs_fini(world)
}

/// @brief Wrapper path that builds query terms from an expression string.
def layer_2b() : void {
    var world : ecs_world_t? = ecs_init()

    // --- ECS_COMPONENT equivalent ---
    let pos_id = ecs_component_register(world, type<Position>)
    let vel_id = ecs_component_register(world, type<Velocity>)

    // --- Build query via expression string ---
    let q = ecs_query_str(world, "Position, [in] Velocity") 

    // --- Create entities with ecs_entity_new + ecs_set ---
    let e1 = ecs_entity_new(world, "e1")
    ecs_set(world, e1, Position(x=10.0d, y=20.0d))
    ecs_set(world, e1, Velocity(x=1.0d, y=2.0d))

    let e2 = ecs_entity_new(world, "e2")
    ecs_set(world, e2, Position(x=10.0d, y=20.0d))
    ecs_set(world, e2, Velocity(x=3.0d, y=4.0d))

    // e3: Position only — will not match the query
    let e3 = ecs_entity_new(world, "e3")
    ecs_set(world, e3, Position(x=10.0d, y=20.0d))

    // --- Iterate using ecs_field ---
    var it = ecs_query_iter(world, q)
    unsafe {
        while (ecs_query_next_iter(it)) {
            // var p : iterator<Position> <- ecs_field_iter(it, type<Position>, 0)
            var p : Position? = ecs_field_get(it, type<Position>, 0)
            var v : Velocity? = ecs_field_get(it, type<Velocity>, 1)
            // for (i, pi, vi in range(it.count), each(p), each(v)) {
            for (i in range(it.count)) {
                p[i].x += v[i].x
                p[i].y += v[i].y
                let name = ecs_get_name(world, it.entities[i])
                print("{name}: \{\{ {p[i].x}, {p[i].y} \}\}\n")
            }
        }
    }

    ecs_query_fini(q)
    ecs_fini(world)
}