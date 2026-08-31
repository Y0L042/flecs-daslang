/// @file
/// @brief Main binding module implementation for the flecs daScript bridge.
///
/// This file mixes handwritten glue code with generated registration fragments.
#include "module_flecs.h"
#include <daScript/daScript.h>

#include <flecs.h>
#include <flecs/addons/flecs_c.h>

#include "scripts/flecs.das.inc"
#include "scripts/flecs_c.das.inc"
#include "scripts/flecs_helpers.das.inc"

/// @name Hand-written helpers
/// Helpers in this section adapt Flecs APIs to the datatypes that daScript
/// exposes more naturally.
/// @{

/// Assign the query expression string on a query descriptor.
static void flecs_set_query_expr(ecs_query_desc_t &desc, const char *expr)
{
    desc.expr = expr;
}

/// Call ecs_set_id using ecs_size_t, which matches daScript typeinfo(sizeof ...).
static void flecs_set_id(ecs_world_t *world, ecs_entity_t entity, ecs_id_t id, ecs_size_t size, void *ptr)
{
    ecs_set_id(world, entity, id, (size_t)size, ptr);
}

/// Call ecs_field_w_size using ecs_size_t instead of size_t.
static void *flecs_field_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index)
{
    return ecs_field_w_size(it, (size_t)size, index);
}

/// Provide the self-owned field accessor shape expected by the Daslang layer.
static void *flecs_field_self_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index)
{
    return ecs_field_w_size(it, (size_t)size, index);
}

/// Call ecs_field_at_w_size using ecs_size_t instead of size_t.
static void *flecs_field_at_w_size(ecs_iter_t *it, ecs_size_t size, int8_t index, int32_t row)
{
    return ecs_field_at_w_size(it, (size_t)size, index, row);
}

/// Set a term in ecs_query_desc_t by index.
/// Required because terms[] is a fixed C array, not directly indexable from Daslang.
static void flecs_query_desc_set_term(ecs_query_desc_t &desc, int32_t idx, ecs_id_t id, int32_t inout)
{
    desc.terms[idx].id = id;
    desc.terms[idx].inout = (ecs_inout_kind_t)inout;
}

/// Set a term with all three fields: id, inout, and oper.
/// Used by query builder methods that need to control the operator (e.g. without() uses EcsNot).
static void flecs_query_desc_set_term_full(ecs_query_desc_t &desc, int32_t idx, ecs_id_t id, int32_t inout, int32_t oper)
{
    desc.terms[idx].id = id;
    desc.terms[idx].inout = (ecs_inout_kind_t)inout;
    desc.terms[idx].oper = (ecs_oper_kind_t)oper;
}

/// Set a term including its source reference and traversal relationship.
/// src_id carries the traversal bit flags (EcsUp / EcsCascade / EcsSelf / EcsDesc)
/// OR'd together. In flecs 4.1.6 ecs_term_ref_t has no separate flags field, so
/// the flags live in the id. A trav of 0 lets flecs default to EcsChildOf.
static void flecs_query_desc_set_term_src(ecs_query_desc_t &desc, int32_t idx, ecs_id_t id, int32_t inout,
                                          int32_t oper, uint64_t src_id, ecs_entity_t trav)
{
    desc.terms[idx].id = id;
    desc.terms[idx].inout = (ecs_inout_kind_t)inout;
    desc.terms[idx].oper = (ecs_oper_kind_t)oper;
    desc.terms[idx].src.id = src_id;
    desc.terms[idx].trav = trav;
}

/// Lookup a path using the Flecs path separator while keeping the prefix null.
static ecs_entity_t flecs_lookup_path(const ecs_world_t *world, ecs_entity_t parent, const char *path, bool recursive)
{
    return ecs_lookup_path_w_sep(world, parent, path, "::", NULL, recursive);
}
/// Resolve a path from a parent entity with the Flecs path separator.
static char *flecs_get_path_from(const ecs_world_t *world, ecs_entity_t parent, ecs_entity_t child)
{
    return ecs_get_path_w_sep(world, parent, child, "::", NULL);
}
/// Create an entity from a path using the Flecs path separator.
static ecs_entity_t flecs_new_from_path(ecs_world_t *world, ecs_entity_t parent, const char *path)
{
    return ecs_new_from_path_w_sep(world, parent, path, "::", NULL);
}
/// Add a path to an entity using the Flecs path separator.
static ecs_entity_t flecs_add_path(ecs_world_t *world, ecs_entity_t entity, ecs_entity_t parent, const char *path)
{
    return ecs_add_path_w_sep(world, entity, parent, path, "::", NULL);
}

/// Set a term on the nested query inside ecs_observer_desc_t by index.
/// Mirrors flecs_query_desc_set_term_full for the observer descriptor.
static void flecs_observer_desc_set_term_full(
    ecs_observer_desc_t &desc, int32_t idx, ecs_id_t id, int32_t inout, int32_t oper)
{
    desc.query.terms[idx].id    = id;
    desc.query.terms[idx].inout = (ecs_inout_kind_t)inout;
    desc.query.terms[idx].oper  = (ecs_oper_kind_t)oper;
}

/// Mirror of flecs_query_desc_set_term_src for the observer descriptor.
static void flecs_observer_desc_set_term_src(ecs_observer_desc_t &desc, int32_t idx, ecs_id_t id, int32_t inout,
                                             int32_t oper, uint64_t src_id, ecs_entity_t trav)
{
    desc.query.terms[idx].id     = id;
    desc.query.terms[idx].inout  = (ecs_inout_kind_t)inout;
    desc.query.terms[idx].oper   = (ecs_oper_kind_t)oper;
    desc.query.terms[idx].src.id = src_id;
    desc.query.terms[idx].trav   = trav;
}

/// Set cache_kind on ecs_query_desc_t.
static void flecs_query_desc_set_cache_kind(ecs_query_desc_t &desc, int32_t cache_kind)
{
    desc.cache_kind = (ecs_query_cache_kind_t)cache_kind;
}

/// Return the i-th id from an ecs_type_t array.
/// ecs_type_t.array is a raw C pointer not indexable from daslang, so this helper bridges the gap.
static ecs_entity_t flecs_type_get_id(const ecs_type_t *type, int32_t index)
{
    if (type == nullptr || type->array == nullptr || index < 0 || index >= type->count)
        return 0;
    return type->array[index];
}

/// Wrappers for ecs_pair_first/second — these are macros in flecs so cannot be bound directly.
static ecs_entity_t flecs_pair_first(ecs_world_t *world, ecs_entity_t pair)
{
    return ecs_pair_first(world, pair);
}

static ecs_entity_t flecs_pair_second(ecs_world_t *world, ecs_entity_t pair)
{
    return ecs_pair_second(world, pair);
}

/// Return the raw low 32 bits of a pair's target.
/// Needed for *value* pairs (ECS_VALUE_PAIR, e.g. (ParentDepth, @3)), where the
/// target encodes a plain integer rather than an entity id. flecs_pair_second()
/// must not be used for these: it runs ecs_get_alive() on the target, which is
/// meaningless for an integer value and yields 0.
static uint64_t flecs_pair_second_value(ecs_id_t pair)
{
    return (uint64_t)ECS_PAIR_SECOND(pair);
}

/// Hierarchy depth of an entity in a Parent (non-fragmenting) hierarchy.
/// Reads the (ParentDepth, @depth) value pair that flecs maintains on children
/// of a Parent hierarchy. Returns 0 for roots, and for ChildOf children (which
/// do not carry a ParentDepth pair).
static int32_t flecs_get_parent_depth(const ecs_world_t *world, ecs_entity_t entity)
{
    const ecs_type_t *type = ecs_get_type(world, entity);
    if (!type)
        return 0;
    for (int32_t i = 0; i < type->count; i++)
    {
        ecs_id_t id = type->array[i];
        if (ECS_IS_PAIR(id) && ECS_PAIR_FIRST(id) == EcsParentDepth)
            return (int32_t)ECS_PAIR_SECOND(id);
    }
    return 0;
}

/// @}

/// @name System callback bridge
/// flecs systems take a C function pointer (ecs_iter_action_t). A daslang
/// callback is not a C function, so we register one fixed C trampoline and stash
/// the daslang callback + Context in the system's callback_ctx. The trampoline
/// recovers them and re-enters daslang each time flecs runs the system.
/// @{

namespace
{
/// Per-system holder kept in ecs_system_desc_t::callback_ctx. Holds either a
/// daslang Func (no-capture, @@fn) or a GC-pinned capturing lambda, plus the
/// owning Context. Freed via flecs_das_system_ctx_free on system destruct/ecs_fini.
struct FlecsDasSystemCtx
{
    das::Context *context = nullptr;
    das::Func func;                     ///< set for the Func path
    das::GcRootLambda lambda;           ///< set for the capturing-lambda path (pins the capture)
    bool useLambda = false;
};

/// ecs_iter_action_t: flecs calls this each time the system runs (once per
/// matched table). Forwards the iterator to the stored daslang callback.
static void flecs_das_system_trampoline(ecs_iter_t *it)
{
    auto *c = static_cast<FlecsDasSystemCtx *>(it->callback_ctx);
    if (!c || !c->context)
        return;
    if (c->useLambda)
        das::das_invoke_lambda<void>::invoke<ecs_iter_t *>(c->context, nullptr, c->lambda, it);
    else
        das::das_invoke_function<void>::invoke<ecs_iter_t *>(c->context, nullptr, c->func, it);
}

/// ecs_ctx_free_t: flecs calls this when the system entity is destroyed
/// (including ecs_fini). Deleting the holder runs ~GcRootLambda (unpins capture).
static void flecs_das_system_ctx_free(void *ptr)
{
    delete static_cast<FlecsDasSystemCtx *>(ptr);
}

/// Build the entity + system descriptors shared by both create overloads and
/// register the system with the trampoline. add_ids must outlive ecs_system_init.
static ecs_entity_t flecs_das_system_init(
    ecs_world_t *world, const char *name, ecs_entity_t phase, const ecs_query_desc_t *query_desc,
    FlecsDasSystemCtx *cbCtx)
{
    ecs_entity_desc_t edesc = {};
    edesc.name = name;
    // ECS_SYSTEM semantics: a phased system adds (DependsOn, phase) + phase.
    // ecs_entity_desc_t.add is a 0-terminated ecs_id_t array.
    ecs_id_t add_ids[3] = {0, 0, 0};
    int n = 0;
    if (phase)
    {
        add_ids[n++] = ecs_pair(EcsDependsOn, phase);
        add_ids[n++] = phase;
    }
    edesc.add = add_ids;

    ecs_system_desc_t sdesc = {};
    sdesc.entity = ecs_entity_init(world, &edesc);
    sdesc.query = *query_desc;
    sdesc.callback = flecs_das_system_trampoline;
    sdesc.callback_ctx = cbCtx;
    sdesc.callback_ctx_free = flecs_das_system_ctx_free;
    return ecs_system_init(world, &sdesc);
}
} // namespace

/// Register a flecs system whose callback is a daslang function pointer (@@fn).
/// phase: e.g. EcsOnUpdate, or 0 for a manual-run-only system (no pipeline phase).
/// The callback receives the ecs_iter_t. Returns the system entity id (0 on failure).
static ecs_entity_t flecs_system_create_fn(
    ecs_world_t *world, const char *name, ecs_entity_t phase, const ecs_query_desc_t *query_desc,
    das::Func func, das::Context *ctx)
{
    auto *c = new FlecsDasSystemCtx();
    c->context = ctx;
    c->func = func;
    c->useLambda = false;
    return flecs_das_system_init(world, name, phase, query_desc, c);
}

/// Like flecs_system_create_fn, but accepts a capturing daslang lambda. The
/// capture is GC-pinned for the system's lifetime and released on destruct.
static ecs_entity_t flecs_system_create_lambda(
    ecs_world_t *world, const char *name, ecs_entity_t phase, const ecs_query_desc_t *query_desc,
    das::TLambda<void, ecs_iter_t *> lambda, das::Context *ctx)
{
    auto *c = new FlecsDasSystemCtx();
    c->context = ctx;
    c->lambda = das::GcRootLambda(lambda, ctx);
    c->useLambda = true;
    return flecs_das_system_init(world, name, phase, query_desc, c);
}

/// @}

/// @name Hand-written annotations
/// These wrappers stay handwritten because the generator skips them.
/// @{

MAKE_TYPE_FACTORY(ecs_world_t, ecs_world_t);
/// Annotation for opaque ecs_world_t values in daScript.
struct EcsWorldAnnotation : das::DummyTypeAnnotation
{
    EcsWorldAnnotation() : DummyTypeAnnotation("ecs_world_t", "ecs_world_t", sizeof(void *), alignof(void *))
    {
    }
};

MAKE_TYPE_FACTORY(ecs_type_t, ecs_type_t);
/// Managed annotation for ecs_type_t, exposing array/count fields.
struct EcsTypeAnnotation : das::ManagedStructureAnnotation<ecs_type_t, false>
{
    EcsTypeAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_type_t", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(array)>("array", "array");
        addField<DAS_BIND_MANAGED_FIELD(count)>("count", "count");
    }
};

MAKE_TYPE_FACTORY(ecs_entities_t, ecs_entities_t);
/// Managed annotation for ecs_entities_t, exposing ids/count metadata.
struct EcsEntitesAnnotation : das::ManagedStructureAnnotation<ecs_entities_t, false>
{
    EcsEntitesAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_entities_t", ml)
    {
        addField<DAS_BIND_MANAGED_FIELD(ids)>("ids", "ids");
        addField<DAS_BIND_MANAGED_FIELD(count)>("count", "count");
        addField<DAS_BIND_MANAGED_FIELD(alive_count)>("alive_count", "alive_count");
    }
};

/// @}

/// Generated annotations are included verbatim from the binding generator.
#include "generated/module_flecs_annotations.inc"
// NOTE: generated/module_flecs_constants.inc is intentionally NOT included.
// It is a leftover from an earlier codegen strategy that emitted per-constant
// getter functions (flecs_const_X()); the generator now emits addConstant()
// calls directly into module_flecs_register.inc. Nothing referenced those 86
// statics, and the file no longer compiles against flecs 4.1.6 because it
// returns EcsPrivate, which was removed. Delete the file once confirmed unused.

/// daScript module entry point for the flecs binding package.
class Module_flecs : public das::Module
{
    bool initialized = false;

  public:
    Module_flecs() : Module("flecs_core")
    {
    }

    bool initDependencies() override
    {
        if (initialized)
            return true;
        initialized = true;

        das::ModuleLibrary lib(this);
        lib.addBuiltInModule();

        // Hand-written registrations
        addAnnotation(new EcsWorldAnnotation());
        addAnnotation(new EcsTypeAnnotation(lib));
        addAnnotation(new EcsEntitesAnnotation(lib));

        das::addExtern<DAS_BIND_FUN(ecs_init)>(*this, lib, "ecs_init", das::SideEffects::modifyExternal, "ecs_init");
        das::addExtern<DAS_BIND_FUN(ecs_fini)>(*this, lib, "ecs_fini", das::SideEffects::modifyExternal, "ecs_fini");
        das::addExtern<DAS_BIND_FUN(ecs_get_entities), das::SimNode_ExtFuncCallAndCopyOrMove>(
            *this, lib, "ecs_get_entities", das::SideEffects::modifyExternal, "ecs_get_entities");

        // System bindings (handwritten: ecs_system_init carries an ecs_iter_action_t
        // fn-ptr, so the generator skips it). SideEffects::invoke because the system
        // callback re-enters daslang; the trailing Context* is injected by addExtern.
        // Generated registrations
#include "generated/module_flecs_register.inc"

        // System bindings (handwritten: ecs_system_init carries an ecs_iter_action_t
        // fn-ptr, so the generator skips it). These must be registered AFTER the
        // generated register include, because flecs_system_create_* reference
        // ecs_query_desc_t in their signatures and that handle annotation is added
        // by the generated include (EcsQueryDescAnnotation). SideEffects::invoke
        // because the system callback re-enters daslang; the trailing Context* is
        // injected by addExtern.
        das::addExtern<DAS_BIND_FUN(flecs_system_create_fn)>(
            *this, lib, "flecs_system_create_fn", das::SideEffects::invoke, "flecs_system_create_fn");
        das::addExtern<DAS_BIND_FUN(flecs_system_create_lambda)>(
            *this, lib, "flecs_system_create_lambda", das::SideEffects::invoke, "flecs_system_create_lambda");
        das::addExtern<DAS_BIND_FUN(ecs_run)>(*this, lib, "ecs_run", das::SideEffects::modifyExternal, "ecs_run");
        das::addExtern<DAS_BIND_FUN(ecs_progress)>(
            *this, lib, "ecs_progress", das::SideEffects::modifyExternal, "ecs_progress");

        // Hand-written helpers
        das::addExtern<DAS_BIND_FUN(flecs_set_query_expr)>(*this, lib, "ecs_query_desc_set_expr",
                                                           das::SideEffects::modifyArgument, "flecs_set_query_expr");
        das::addExtern<DAS_BIND_FUN(flecs_set_id)>(*this, lib, "flecs_set_id", das::SideEffects::modifyExternal,
                                                   "flecs_set_id");
        das::addExtern<DAS_BIND_FUN(flecs_field_w_size)>(*this, lib, "flecs_field_w_size",
                                                         das::SideEffects::modifyExternal, "flecs_field_w_size");
        das::addExtern<DAS_BIND_FUN(flecs_field_self_w_size)>(
            *this, lib, "flecs_field_self_w_size", das::SideEffects::modifyExternal, "flecs_field_self_w_size");
        das::addExtern<DAS_BIND_FUN(flecs_field_at_w_size)>(*this, lib, "flecs_field_at_w_size",
                                                            das::SideEffects::modifyExternal, "flecs_field_at_w_size");
        das::addExtern<DAS_BIND_FUN(flecs_query_desc_set_term)>(
            *this, lib, "flecs_query_desc_set_term", das::SideEffects::modifyArgument, "flecs_query_desc_set_term");
        das::addExtern<DAS_BIND_FUN(flecs_query_desc_set_term_full)>(
            *this, lib, "flecs_query_desc_set_term_full", das::SideEffects::modifyArgument, "flecs_query_desc_set_term_full");
        das::addExtern<DAS_BIND_FUN(flecs_observer_desc_set_term_full)>(
            *this, lib, "flecs_observer_desc_set_term_full", das::SideEffects::modifyArgument,
            "flecs_observer_desc_set_term_full")
            ->args({"desc", "idx", "id", "inout", "oper"});
        das::addExtern<DAS_BIND_FUN(flecs_query_desc_set_term_src)>(
            *this, lib, "flecs_query_desc_set_term_src", das::SideEffects::modifyArgument,
            "flecs_query_desc_set_term_src")
            ->args({"desc", "idx", "id", "inout", "oper", "src_id", "trav"});
        das::addExtern<DAS_BIND_FUN(flecs_observer_desc_set_term_src)>(
            *this, lib, "flecs_observer_desc_set_term_src", das::SideEffects::modifyArgument,
            "flecs_observer_desc_set_term_src")
            ->args({"desc", "idx", "id", "inout", "oper", "src_id", "trav"});

        // Query traversal flags. These are #define macros in flecs.h rather than
        // FLECS_API extern const declarations, so gen_flecs_bindings.py cannot see
        // them and never emits them into module_flecs_register.inc. They are uint64
        // bit flags (bits 59..63) OR'd into ecs_term_t::src.id -- registering them as
        // int would silently truncate every one of them to zero.
        addConstant(*this, "EcsSelf", (uint64_t)EcsSelf);
        addConstant(*this, "EcsUp", (uint64_t)EcsUp);
        addConstant(*this, "EcsTrav", (uint64_t)EcsTrav);
        addConstant(*this, "EcsCascade", (uint64_t)EcsCascade);
        addConstant(*this, "EcsDesc", (uint64_t)EcsDesc);
        das::addExtern<DAS_BIND_FUN(flecs_query_desc_set_cache_kind)>(*this, lib, "flecs_query_desc_set_cache_kind",
                                                                      das::SideEffects::modifyArgument,
                                                                      "flecs_query_desc_set_cache_kind");
        das::addExtern<DAS_BIND_FUN(flecs_lookup_path)>(*this, lib, "flecs_lookup_path", das::SideEffects::none,
                                                        "flecs_lookup_path");
        das::addExtern<DAS_BIND_FUN(flecs_get_path_from), das::SimNode_ExtFuncCallAndCopyOrMove>(
            *this, lib, "flecs_get_path_from", das::SideEffects::none, "flecs_get_path_from");
        das::addExtern<DAS_BIND_FUN(flecs_new_from_path)>(*this, lib, "flecs_new_from_path",
                                                          das::SideEffects::modifyExternal, "flecs_new_from_path");
        das::addExtern<DAS_BIND_FUN(flecs_add_path)>(*this, lib, "flecs_add_path", das::SideEffects::modifyExternal,
                                                     "flecs_add_path");
        das::addExtern<DAS_BIND_FUN(flecs_type_get_id)>(*this, lib, "flecs_type_get_id", das::SideEffects::none,
                                                        "flecs_type_get_id");
        das::addExtern<DAS_BIND_FUN(flecs_pair_first)>(*this, lib, "flecs_pair_first", das::SideEffects::none,
                                                       "flecs_pair_first");
        das::addExtern<DAS_BIND_FUN(flecs_pair_second)>(*this, lib, "flecs_pair_second", das::SideEffects::none,
                                                        "flecs_pair_second");
        das::addExtern<DAS_BIND_FUN(flecs_pair_second_value)>(
            *this, lib, "flecs_pair_second_value", das::SideEffects::none, "flecs_pair_second_value")
            ->args({"pair"});
        das::addExtern<DAS_BIND_FUN(flecs_get_parent_depth)>(
            *this, lib, "flecs_get_parent_depth", das::SideEffects::none, "flecs_get_parent_depth")
            ->args({"world", "entity"});

        compileBuiltinModule("flecs.das", flecs_das, sizeof(flecs_das));
        compileBuiltinModule("flecs_c.das", flecs_c_das, sizeof(flecs_c_das));
        // compileBuiltinModule("flecs_helpers.das", flecs_helpers_das, sizeof(flecs_helpers_das));

        return true;
    }
};
REGISTER_MODULE(Module_flecs);
REGISTER_DYN_MODULE(Module_flecs, Module_flecs);
