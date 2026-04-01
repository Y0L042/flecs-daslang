options gen2
require flecs


struct Position {
    x : double
    y : double
}

struct Velocity {
    x : double
    y : double
}

[export]
def sandbox() : void {
    print("=== Flecs Sandbox ===\n")
    print("--- Layer 1: Basic world setup, component registration, query, and iteration ---\n")
    layer_1()
    print("\n--- Layer 2: Placeholder for future tests ---\n")
    layer_2()
    print("\n=== End of Sandbox ===")
}

def layer_1() : void {
    var world : ecs_world_t? = ecs_init()

    // --- Register Position component (ECS_COMPONENT equivalent) ---
    let pos_edesc : ecs_entity_desc_t = ecs_entity_desc_t(name="Position", use_low_id=true)
    var pos_entity : ecs_entity_t
    unsafe { 
        pos_entity = ecs_entity_init(world, addr(pos_edesc)) 
    }

    let pos_cdesc : ecs_component_desc_t = ecs_component_desc_t(
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

def layer_2() : bool {
    return true
}