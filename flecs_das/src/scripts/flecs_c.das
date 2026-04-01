options gen2
// GENERATED — do not edit manually.
// Regenerate with: bash manage.sh gen-flecs-c-bind

require flecs


// ==============================================================

// Category A — Simple _id wrappers

def ecs_add(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t) {
    ecs_add_id(world, entity, id)
}

def ecs_remove(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t) {
    ecs_remove_id(world, entity, id)
}

def ecs_auto_override(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t) {
    ecs_auto_override_id(world, entity, id)
}

def ecs_modified(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t) {
    ecs_modified_id(world, entity, id)
}

def ecs_has(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t) : bool {
    return ecs_has_id(world, entity, id)
}

def ecs_owns(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t) : bool {
    return ecs_owns_id(world, entity, id)
}

def ecs_enable_component(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; enable: bool) {
    ecs_enable_id(world, entity, id, enable)
}

def ecs_is_enabled(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t) : bool {
    return ecs_is_enabled_id(world, entity, id)
}

def ecs_count(world: ecs_world_t?; id: ecs_entity_t) : int {
    return ecs_count_id(world, id)
}

def ecs_get_target_for(world: ecs_world_t?; entity: ecs_entity_t; rel: ecs_entity_t; id: ecs_entity_t) : ecs_entity_t {
    return ecs_get_target_for_id(world, entity, rel, id)
}

def ecs_new_w(world: ecs_world_t?; id: ecs_entity_t) : ecs_entity_t {
    return ecs_new_w_id(world, id)
}

def ecs_bulk_new(world: ecs_world_t?; id: ecs_entity_t; count: int) : ecs_entity_t const? {
    return ecs_bulk_new_w_id(world, id, count)
}

def ecs_each(world: ecs_world_t?; id: ecs_entity_t) : ecs_iter_t {
    return ecs_each_id(world, id)
}

// ==============================================================

// Category B — Pair wrappers

def ecs_add_pair(world: ecs_world_t?; entity: ecs_entity_t; first: ecs_entity_t; second: ecs_entity_t) {
    ecs_add_id(world, entity, ecs_make_pair(first, second))
}

def ecs_remove_pair(world: ecs_world_t?; entity: ecs_entity_t; first: ecs_entity_t; second: ecs_entity_t) {
    ecs_remove_id(world, entity, ecs_make_pair(first, second))
}

def ecs_auto_override_pair(world: ecs_world_t?; entity: ecs_entity_t; first: ecs_entity_t; second: ecs_entity_t) {
    ecs_auto_override_id(world, entity, ecs_make_pair(first, second))
}

def ecs_modified_pair(world: ecs_world_t?; entity: ecs_entity_t; first: ecs_entity_t; second: ecs_entity_t) {
    ecs_modified_id(world, entity, ecs_make_pair(first, second))
}

def ecs_has_pair(world: ecs_world_t?; entity: ecs_entity_t; first: ecs_entity_t; second: ecs_entity_t) : bool {
    return ecs_has_id(world, entity, ecs_make_pair(first, second))
}

def ecs_owns_pair(world: ecs_world_t?; entity: ecs_entity_t; first: ecs_entity_t; second: ecs_entity_t) : bool {
    return ecs_owns_id(world, entity, ecs_make_pair(first, second))
}

def ecs_enable_pair(world: ecs_world_t?; entity: ecs_entity_t; first: ecs_entity_t; second: ecs_entity_t; enable: bool) {
    ecs_enable_id(world, entity, ecs_make_pair(first, second), enable)
}

def ecs_is_enabled_pair(world: ecs_world_t?; entity: ecs_entity_t; first: ecs_entity_t; second: ecs_entity_t) : bool {
    return ecs_is_enabled_id(world, entity, ecs_make_pair(first, second))
}

def ecs_new_w_pair(world: ecs_world_t?; first: ecs_entity_t; second: ecs_entity_t) : ecs_entity_t {
    return ecs_new_w_id(world, ecs_make_pair(first, second))
}

def ecs_each_pair(world: ecs_world_t?; first: ecs_entity_t; second: ecs_entity_t) : ecs_iter_t {
    return ecs_each_id(world, ecs_make_pair(first, second))
}

// ==============================================================

// Category C — Pointer-returning generics

def ecs_get(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; _t: type<auto(T)>) : T? const {
    unsafe { return reinterpret<T? const>(ecs_get_id(world, entity, id)); }
}

def ecs_get_mut(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; _t: type<auto(T)>) : T? {
    unsafe { return reinterpret<T?>(ecs_get_mut_id(world, entity, id)); }
}

def ecs_ensure(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; _t: type<auto(T)>) : T? {
    unsafe { return reinterpret<T?>(ecs_ensure_id(world, entity, id, typeinfo sizeof(type<T>))); }
}

def ecs_ref_get(world: ecs_world_t?; ref: ecs_ref_t?; id: ecs_entity_t; _t: type<auto(T)>) : T? {
    unsafe { return reinterpret<T?>(ecs_ref_get_id(world, ref, id)); }
}

def ecs_table_get(world: ecs_world_t?; tbl: ecs_table_t?; id: ecs_entity_t; offset: int; _t: type<auto(T)>) : T? {
    unsafe { return reinterpret<T?>(ecs_table_get_id(world, tbl, id, offset)); }
}

def ecs_emplace(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; is_new: bool?; _t: type<auto(T)>) : T? {
    unsafe { return reinterpret<T?>(ecs_emplace_id(world, entity, id, typeinfo sizeof(type<T>), is_new)); }
}

// ==============================================================

// Category D — Value-setting

def ecs_set(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; var value: auto) {
    unsafe { flecs_set_id(world, entity, id, typeinfo sizeof(value), addr(value)); }
}

def ecs_set_ptr(world: ecs_world_t?; entity: ecs_entity_t; id: ecs_entity_t; size: int; ptr: void?) {
    unsafe { flecs_set_id(world, entity, id, size, ptr); }
}

// ==============================================================

// Category E — Iterator field access

// Read-only access (T? const result when indexed).
def ecs_field(var it: ecs_iter_t; _t: type<auto(T)>; index: int8) : T? {
    unsafe { return reinterpret<T?>(flecs_field_w_size(addr(it), typeinfo sizeof(type<T>), index)); }
}

def ecs_field_self(var it: ecs_iter_t; _t: type<auto(T)>; index: int8) : T? {
    unsafe { return reinterpret<T?>(flecs_field_self_w_size(addr(it), typeinfo sizeof(type<T>), index)); }
}

def ecs_field_at(var it: ecs_iter_t; _t: type<auto(T)>; index: int8; row: int) : T? {
    unsafe { return reinterpret<T?>(flecs_field_at_w_size(addr(it), typeinfo sizeof(type<T>), index, row)); }
}

// Mutable access — must be called from within an unsafe block.
// Returns void? so the caller reinterprets in their own unsafe scope,
// which is what enables p[i].field += ... to compile without "left side can't be constant".
// Usage:
//   unsafe {
//       var p = reinterpret<Position?>(ecs_field_mut(addr(it), typeinfo sizeof(type<Position>), int8(0)))
//       p[i].x += v[i].x
//   }
def ecs_field_mut(it: ecs_iter_t?; size: int; index: int8) : void? {
    unsafe { return flecs_field_w_size(it, size, index); }
}

def ecs_field_self_mut(it: ecs_iter_t?; size: int; index: int8) : void? {
    unsafe { return flecs_field_self_w_size(it, size, index); }
}

def ecs_field_at_mut(it: ecs_iter_t?; size: int; index: int8; row: int) : void? {
    unsafe { return flecs_field_at_w_size(it, size, index, row); }
}

// ==============================================================

// Category F — Component registration  (replaces ECS_COMPONENT boilerplate)

def ecs_component_register(world: ecs_world_t?; _t: type<auto(T)>) : ecs_entity_t {
    let tname = typeinfo stripped_typename(type<T>)
    let edesc = ecs_entity_desc_t(name=tname, symbol=tname, use_low_id=true)
    var entity: ecs_entity_t
    unsafe { entity = ecs_entity_init(world, addr(edesc)); }
    var cdesc = ecs_component_desc_t(entity=entity, vtype=ecs_type_info_t(
        size=typeinfo sizeof(type<T>), alignment=typeinfo alignof(type<T>)))
    var id: ecs_entity_t
    unsafe { id = ecs_component_init(world, addr(cdesc)); }
    return id
}

// ==============================================================

// Category G — Init shorthands

def ecs_entity_new(world: ecs_world_t?; name: string) : ecs_entity_t {
    let edesc = ecs_entity_desc_t(name=name)
    var entity: ecs_entity_t
    unsafe { entity = ecs_entity_init(world, addr(edesc)); }
    return entity
}

def ecs_query_str(world: ecs_world_t?; expr: string) : ecs_query_t? {
    var qdesc: ecs_query_desc_t
    ecs_query_desc_set_expr(qdesc, expr)
    var q: ecs_query_t?
    unsafe { q = ecs_query_init(world, addr(qdesc)); }
    return q
}

// ==============================================================

// Category H — Path shorthands  (hardcoded "::" separator, NULL prefix via C wrappers)

def ecs_lookup_from(world: ecs_world_t?; parent: ecs_entity_t; path: string) : ecs_entity_t {
    return flecs_lookup_path(world, parent, path, true)
}

def ecs_get_path_from(world: ecs_world_t?; parent: ecs_entity_t; child: ecs_entity_t) : string const {
    return flecs_get_path_from(world, parent, child)
}

def ecs_get_path(world: ecs_world_t?; child: ecs_entity_t) : string const {
    return flecs_get_path_from(world, 0ul, child)
}

def ecs_new_from_path(world: ecs_world_t?; parent: ecs_entity_t; path: string) : ecs_entity_t {
    return flecs_new_from_path(world, parent, path)
}

def ecs_add_path(world: ecs_world_t?; entity: ecs_entity_t; parent: ecs_entity_t; path: string) : ecs_entity_t {
    return flecs_add_path(world, entity, parent, path)
}

def ecs_add_fullpath(world: ecs_world_t?; entity: ecs_entity_t; path: string) : ecs_entity_t {
    return flecs_add_path(world, entity, 0ul, path)
}
