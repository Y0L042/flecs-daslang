options gen2
require flecs

def ecs_query_builder(world : ecs_world_t?, var terms : array<ecs_term_t>, cache_kind : int) : ecs_query_t? {
    var q_desc : ecs_query_desc_t
    for (i in range(length(terms))) {
        flecs_query_desc_set_term(q_desc, i, terms[i].id, int(terms[i].inout))
    }
    flecs_query_desc_set_cache_kind(q_desc, cache_kind)

    var q : ecs_query_t?
    unsafe { 
        q = ecs_query_init(world, addr(q_desc)) 
    }

    return q
}

def ecs_term(id: ecs_entity_t; inout: int = EcsInOutDefault) : ecs_term_t {
    let term : ecs_term_t = ecs_term_t(id=id, inout=int16(inout))

    return term
}

def ecs_term(world : ecs_world_t?, cmp : type<auto(T)>, inout : int = EcsInOutDefault) : ecs_term_t {
    let id : ecs_entity_t = ecs_component_register(world, type<T>)
    let term : ecs_term_t = ecs_term_t(id=id, inout=int16(inout))

    return term
}

def ecs_set(world : ecs_world_t?, entity : ecs_entity_t, var value : auto(T)) : void {
    ecs_set(world, entity, ecs_component_register(world, type<T>), value)
}

def ecs_query_next_iter(var it: ecs_iter_t&) : bool {
    unsafe { 
        return ecs_query_next(addr(it)); 
    }
}

def ecs_field_get(var it: ecs_iter_t&, var t: type<auto(T)>, index: int) : T? {
    unsafe { 
        return reinterpret<T?>(ecs_field_mut(addr(it), typeinfo sizeof(t), int8(index))); 
    }
}

def ecs_field_iter(var it: ecs_iter_t&, var t: type<auto(T)>, index: int) : iterator<T> {
    unsafe { 
        return <- reinterpret<iterator<T>>(ecs_field_mut(addr(it), typeinfo sizeof(t), int8(index))); 
    }
}