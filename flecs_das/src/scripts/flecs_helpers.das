/// @file
/// @brief Handwritten Daslang helper layer for query, term, and iterator APIs.
///
/// These wrappers smooth over daScript typing and keep common Flecs operations
/// concise for users of the binding package.
options gen2

module flecs_boost shared

require flecs_core public
require daslib/flat_hash_table public
require daslib/match

/*
Callback : lambda<(world : ecs_world_t?, e : ecs_entity_t, var cmp : auto(T)) : void>>

struct Hooks {
    OnAdd : array<Callback>
    OnSet : array<Callback>
    OnRemove : array<Callback>
}

map:
    key     = string name of component type T
    value   = Hooks

add_hook(type<CharacterBodyDesc>, OnAdd, @(world : ecs_world_t?, e : ecs_entity_t, var cmp : CharacterBodyDesc&) : void { print("{world}, {e}, {cmp}"); })
*/

typedef EcsRawCallback = lambda<(var world : ecs_world_t?; var e : ecs_entity_t; cmp : void?) : void>

enum EcsHookEvent {
    ONADD,
    ONSET,
    ONREMOVE
}

struct EcsHooks {
    on_add    : array<EcsRawCallback>
    on_set    : array<EcsRawCallback>
    on_remove : array<EcsRawCallback>
}

// Global hook registry
var ecs_hook_registry : table<string, EcsHooks>

def ecs_add_hook(t : type<auto(T)>; event : EcsHookEvent; var cb : lambda<(var world : ecs_world_t?; var e : ecs_entity_t; cmp : T) : void>) {
    let key = typeinfo typename(type<T>)
    if (!key_exists(ecs_hook_registry, key)) {
        ecs_hook_registry[key] = EcsHooks()
    }
    var raw : EcsRawCallback <- @(var world : ecs_world_t?; var e : ecs_entity_t; cmp_ptr : void?) {
        unsafe {
            invoke(cb, world, e, *reinterpret<T?>(cmp_ptr))
        }
    }
    var hooks & = unsafe(ecs_hook_registry[key])
    if (event == EcsHookEvent.ONADD) {
        hooks.on_add |> emplace(raw)
    } elif (event == EcsHookEvent.ONSET) {
        hooks.on_set |> emplace(raw)
    } elif (event == EcsHookEvent.ONREMOVE) {
        hooks.on_remove |> emplace(raw)
    }
}

def ecs_emit_hook_event(t : type<auto(T)>; event : EcsHookEvent; var world : ecs_world_t?; var e : ecs_entity_t; var cmp : T) {
    let key = typeinfo typename(type<T>)
    if (!key_exists(ecs_hook_registry, key)) {
        return
    }
    var hooks & = unsafe(ecs_hook_registry[key])
    unsafe {
        if (event == EcsHookEvent.ONADD) {
            for (cb in hooks.on_add) {
                invoke(cb, world, e, reinterpret<void?>(addr(cmp)))
            }
        } elif (event == EcsHookEvent.ONSET) {
            for (cb in hooks.on_set) {
                invoke(cb, world, e, reinterpret<void?>(addr(cmp)))
            }
        } elif (event == EcsHookEvent.ONREMOVE) {
            for (cb in hooks.on_remove) {
                invoke(cb, world, e, reinterpret<void?>(addr(cmp)))
            }
        }
    }
}


/// Build a query from an array of prepared terms and a cache policy.
def ecs_query_builder(world : ecs_world_t?, var terms : array<ecs_term_t>, cache_kind : int) : ecs_query_t? {
    var q_desc : ecs_query_desc_t
    for (i in range(length(terms))) {
        // set_term_src (not set_term) so oper, src and trav all survive: the
        // plain setter drops oper, which silently broke optional/not terms.
        flecs_query_desc_set_term_src(q_desc, i, terms[i].id, int(terms[i].inout), int(terms[i].oper),
            terms[i].src.id, terms[i].trav)
    }
    flecs_query_desc_set_cache_kind(q_desc, cache_kind)

    var q : ecs_query_t?
    unsafe {
        q = ecs_query_init(world, addr(q_desc))
    }

    return q
}

/// Create a term from a raw Flecs id.
def ecs_term(id : ecs_entity_t; inout : int = EcsInOutDefault) : ecs_term_t {
    let term : ecs_term_t = ecs_term_t(id = id, inout = int16(inout))

    return term
}

/// Create a term by registering a daScript component type in the current world.
def ecs_term(world : ecs_world_t?, cmp : type<auto(T)>, inout : int = EcsInOutDefault) : ecs_term_t {
    let id : ecs_entity_t = ecs_component_register(world, type<T>)
    let term : ecs_term_t = ecs_term_t(id = id, inout = int16(inout))

    return term
}

def ecs_term(world : ecs_world_t?, cmp : type<auto(T)>, oper : int = EcsAnd, inout : int = EcsInOutDefault) : ecs_term_t {
    let id : ecs_entity_t = ecs_component_register(world, type<T>)
    let term : ecs_term_t = ecs_term_t(id = id, inout = int16(inout), oper = int16(oper))

    return term
}

/// Create a traversal term, reading the component from an ancestor rather than
/// from the matched entity. `flags` is an OR of EcsUp / EcsCascade / EcsSelf /
/// EcsDesc and is written into src.id; a `trav` of 0 lets flecs default to
/// EcsChildOf. Note a non-optional traversal term does not match root entities:
/// pass oper = EcsOptional to keep them, and the field is then unset for them.
def ecs_term_up(world : ecs_world_t?; cmp : type<auto(T)>; flags : uint64; trav : ecs_entity_t = 0ul; oper : int = EcsAnd; inout : int = EcsInOutDefault) : ecs_term_t {
    let id : ecs_entity_t = ecs_component_register(world, type<T>)
    var term : ecs_term_t = ecs_term_t(id = id, inout = int16(inout), oper = int16(oper))
    term.src.id = flags
    term.trav = trav

    return term
}


/// Add a tag to an entity using the registered component type.
def ecs_add(world : ecs_world_t?, entity : ecs_entity_t, var tag : auto(T)) : void {
    ecs_add(world, entity, ecs_component_register(world, type<T>))
}

/// Set a value on an entity using the registered component type.
def ecs_set(world : ecs_world_t?, entity : ecs_entity_t, var value : auto(T)) : void {
    ecs_set(world, entity, ecs_component_register(world, type<T>), value)
}

def ecs_get(world : ecs_world_t?, entity : ecs_entity_t, t : type<auto(T)>) : T? {
// TODO what the actual fuck is this. probably to remove constness or something
    unsafe { return reinterpret<T?>(reinterpret<void?>(ecs_get_id(world, entity, ecs_component_register(world, type<T>)))); }
}

def ecs_get_mut(world : ecs_world_t?, entity : ecs_entity_t, t : type<auto(T)>) : T? {
// TODO what the actual fuck is this. probably to remove constness or something
    unsafe { return reinterpret<T?>(reinterpret<void?>(ecs_get_mut_id(world, entity, ecs_component_register(world, type<T>)))); }
}

/// Advance a query iterator and return whether another row is available.
def ecs_query_next_iter(var it : ecs_iter_t&) : bool {
    unsafe {
        return ecs_query_next(addr(it));
    }
}

/// Read a typed field from the iterator with mutable access in the call site.
def ecs_field_get(var it : ecs_iter_t&, var t : type<auto(T)>, index : int) : T? {
    unsafe {
        return reinterpret<T?>(ecs_field_mut(addr(it), typeinfo sizeof(t), int8(index)));
    }
}

/// Return an iterator over a typed field.
def ecs_field_iter(var it : ecs_iter_t&, var t : type<auto(T)>, index : int) : iterator<T> {
    unsafe {
        return <- reinterpret<iterator<T>>(ecs_field_mut(addr(it), typeinfo sizeof(t), int8(index)));
    }
}

/// Iterate children of `parent`, invoking `blk` once per child entity.
def ecs_children_each(world : ecs_world_t?; parent : ecs_entity_t; blk : block<(child : ecs_entity_t)>) : void {
    var it = ecs_children(world, parent)
    unsafe {
        while (ecs_children_next(addr(it))) {
            for (i in range(it.count)) {
                invoke(blk, it.entities[i])
            }
        }
    }
}

/// Iterate root-level entities, invoking `blk` once per entity.
def ecs_children_each(world : ecs_world_t?; blk : block<(child : ecs_entity_t)>) : void {
    ecs_children_each(world, 0ul, blk)
}

/// Iterate all descendants of `parent` depth-first, invoking `blk` per entity.
/// Children of each node are visited immediately after the node itself.
/// Works with both ChildOf and Parent hierarchy storages.
///
/// The children of a node are collected before recursing, so that the flecs
/// iterator for a given level is exhausted before a nested iteration begins.
def ecs_children_each_recursive(world : ecs_world_t?; parent : ecs_entity_t; blk : block<(child : ecs_entity_t)>) : void {
    var level : array<ecs_entity_t>
    ecs_children_each(world, parent) <| $(child : ecs_entity_t) {
        level |> push(child)
    }
    for (child in level) {
        invoke(blk, child)
        ecs_children_each_recursive(world, child, blk)
    }
    delete level
}

/// Iterate all entities in the world depth-first, starting from the root.
def ecs_children_each_recursive(world : ecs_world_t?; blk : block<(child : ecs_entity_t)>) : void {
    ecs_children_each_recursive(world, 0ul, blk)
}

/// Collect the children of `parent` into an array (ChildOf or Parent storage).
def ecs_children_array(world : ecs_world_t?; parent : ecs_entity_t) : array<ecs_entity_t> {
    var result : array<ecs_entity_t>
    ecs_children_each(world, parent) <| $(child : ecs_entity_t) {
        result |> push(child)
    }
    return <- result
}

/// Number of direct children of `parent`.
def ecs_children_count(world : ecs_world_t?; parent : ecs_entity_t) : int {
    var n = 0
    ecs_children_each(world, parent) <| $(child : ecs_entity_t) {
        n++
    }
    return n
}

// ==============================================================
// Ordered children (EcsOrderedChildren trait)
// ==============================================================

/// Children of `parent` in stable order, as an array.
/// Requires the EcsOrderedChildren trait on `parent`; without it flecs fails
/// the underlying call, so callers should add the trait first.
/// Parent-storage children are always ordered, as that storage is built on
/// top of the same ordered-children vector.
def ecs_ordered_children_array(world : ecs_world_t?; parent : ecs_entity_t) : array<ecs_entity_t> {
    var result : array<ecs_entity_t>
    let children = ecs_get_ordered_children(world, parent)
    unsafe {
        for (i in range(children.count)) {
            result |> push(children.ids[i])
        }
    }
    return <- result
}

/// Iterate children of `parent` in stable order. See ecs_ordered_children_array.
def ecs_ordered_children_each(world : ecs_world_t?; parent : ecs_entity_t; blk : block<(child : ecs_entity_t)>) : void {
    let children = ecs_get_ordered_children(world, parent)
    unsafe {
        for (i in range(children.count)) {
            invoke(blk, children.ids[i])
        }
    }
}

/// Mark `parent` as preserving child order, so ecs_children/ordered accessors
/// return children in creation (or explicitly set) order.
def ecs_mark_ordered_children(world : ecs_world_t?; parent : ecs_entity_t) : void {
    ecs_add_id(world, parent, EcsOrderedChildren)
}

/// Set an explicit child order on `parent`. `parent` must have the
/// EcsOrderedChildren trait, and `children` must contain exactly its children.
def ecs_set_children_order(world : ecs_world_t?; parent : ecs_entity_t; var children : array<ecs_entity_t>) : void {
    if (length(children) == 0) {
        return
    }
    unsafe {
        ecs_set_child_order(world, parent, addr(children[0]), length(children))
    }
}

// ==============================================================
// Parent-storage depth (EcsParentDepth)
// ==============================================================

/// Hierarchy depth of an entity in a Parent-storage hierarchy.
/// Flecs adds a (ParentDepth, @depth) *value* pair to children in a Parent
/// hierarchy; this reads that depth back. Returns 0 when the entity has no
/// such pair (a root, or a ChildOf child, which does not use ParentDepth).
///
/// Backed by a C++ helper: the depth is the raw low bits of the pair target,
/// so it cannot be read with ecs_pair_second(), which resolves its target as
/// an entity id via ecs_get_alive().
def ecs_get_parent_depth(world : ecs_world_t?; entity : ecs_entity_t) : int {
    return flecs_get_parent_depth(world, entity)
}

/// Iterate each component/tag id of an entity, invoking blk once per id.
def ecs_entity_each(world : ecs_world_t?; entity : ecs_entity_t; blk : block<(id : ecs_entity_t)>) : void {
    let typ = ecs_get_type(world, entity)
    if (typ == null) {
        return
    }
    for (i in range(typ.count)) {
        invoke(blk, flecs_type_get_id(typ, i))
    }
}

// ==============================================================

/// Create an observer from a list of terms and a single event.
/// Returns the observer entity id (0 on failure).
def ecs_observer_builder(
                         world     : ecs_world_t?;
                         evt       : ecs_entity_t;
                         var terms : array<ecs_term_t>
                         ) : ecs_entity_t {
    var obs_desc : ecs_observer_desc_t
    obs_desc.events[0] = evt
    for (i in range(length(terms))) {
        flecs_observer_desc_set_term_src(obs_desc, i, terms[i].id, int(terms[i].inout), int(terms[i].oper),
            terms[i].src.id, terms[i].trav)
    }
    var obs_id : ecs_entity_t = 0ul
    unsafe {
        obs_id = ecs_observer_init(world, addr(obs_desc))
    }
    return obs_id
}

/// Iterate all entities in an ecs_iter_t, invoking blk once per entity.
def ecs_observer_each(var it : ecs_iter_t&; blk : block<(entity : ecs_entity_t)>) : void {
    for (i in range(it.count)) {
        invoke(blk, it.entities[i])
    }
}

// ==============================================================

/// Fill an ecs_query_desc_t from prepared terms. Shared by the system builders.
def private ecs_system_fill_query(var q_desc : ecs_query_desc_t; var terms : array<ecs_term_t>) : void {
    for (i in range(length(terms))) {
        flecs_query_desc_set_term_src(q_desc, i, terms[i].id, int(terms[i].inout), int(terms[i].oper),
            terms[i].src.id, terms[i].trav)
    }
}

/// Register a flecs system whose callback is a daslang function pointer (@@fn).
/// The callback runs once per matched table; iterate via it.count / it.entities /
/// ecs_field_get. phase: e.g. EcsOnUpdate, or 0ul for a manual-run-only system.
/// Returns the system entity id (0 on failure).
def ecs_system_builder(
                       world     : ecs_world_t?;
                       name      : string;
                       phase     : ecs_entity_t;
                       var terms : array<ecs_term_t>;
                       cb        : function<(var it : ecs_iter_t?) : void>
                       ) : ecs_entity_t {
    var q_desc : ecs_query_desc_t
    ecs_system_fill_query(q_desc, terms)
    var sys : ecs_entity_t = 0ul
    unsafe {
        sys = flecs_system_create_fn(world, name, phase, addr(q_desc), cb)
    }
    return sys
}

/// Like ecs_system_builder, but takes a capturing lambda. The capture is
/// GC-pinned for the system's lifetime and released when the system is destroyed.
def ecs_system_builder(
                       world     : ecs_world_t?;
                       name      : string;
                       phase     : ecs_entity_t;
                       var terms : array<ecs_term_t>;
                       cb        : lambda<(var it : ecs_iter_t?) : void>
                       ) : ecs_entity_t {
    var q_desc : ecs_query_desc_t
    ecs_system_fill_query(q_desc, terms)
    var sys : ecs_entity_t = 0ul
    unsafe {
        sys = flecs_system_create_lambda(world, name, phase, addr(q_desc), cb)
    }
    return sys
}
