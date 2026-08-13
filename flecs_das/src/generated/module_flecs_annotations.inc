// GENERATED - do not edit manually.
// Regenerate with: bash manage.sh gen-flecs-bind

// --- Opaque types (DummyTypeAnnotation) ---

MAKE_TYPE_FACTORY(ecs_stage_t, ecs_stage_t);
struct EcsStageAnnotation : das::DummyTypeAnnotation {
    EcsStageAnnotation()
        : DummyTypeAnnotation("ecs_stage_t", "ecs_stage_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_table_t, ecs_table_t);
struct EcsTableAnnotation : das::DummyTypeAnnotation {
    EcsTableAnnotation()
        : DummyTypeAnnotation("ecs_table_t", "ecs_table_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_observable_t, ecs_observable_t);
struct EcsObservableAnnotation : das::DummyTypeAnnotation {
    EcsObservableAnnotation()
        : DummyTypeAnnotation("ecs_observable_t", "ecs_observable_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_ref_t, ecs_ref_t);
struct EcsRefAnnotation : das::DummyTypeAnnotation {
    EcsRefAnnotation()
        : DummyTypeAnnotation("ecs_ref_t", "ecs_ref_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_record_t, ecs_record_t);
struct EcsRecordAnnotation : das::DummyTypeAnnotation {
    EcsRecordAnnotation()
        : DummyTypeAnnotation("ecs_record_t", "ecs_record_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_component_record_t, ecs_component_record_t);
struct EcsComponentRecordAnnotation : das::DummyTypeAnnotation {
    EcsComponentRecordAnnotation()
        : DummyTypeAnnotation("ecs_component_record_t", "ecs_component_record_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_mixins_t, ecs_mixins_t);
struct EcsMixinsAnnotation : das::DummyTypeAnnotation {
    EcsMixinsAnnotation()
        : DummyTypeAnnotation("ecs_mixins_t", "ecs_mixins_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_table_record_t, ecs_table_record_t);
struct EcsTableRecordAnnotation : das::DummyTypeAnnotation {
    EcsTableRecordAnnotation()
        : DummyTypeAnnotation("ecs_table_record_t", "ecs_table_record_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_map_t, ecs_map_t);
struct EcsMapAnnotation : das::DummyTypeAnnotation {
    EcsMapAnnotation()
        : DummyTypeAnnotation("ecs_map_t", "ecs_map_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_strbuf_t, ecs_strbuf_t);
struct EcsStrbufAnnotation : das::DummyTypeAnnotation {
    EcsStrbufAnnotation()
        : DummyTypeAnnotation("ecs_strbuf_t", "ecs_strbuf_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_table_range_t, ecs_table_range_t);
struct EcsTableRangeAnnotation : das::DummyTypeAnnotation {
    EcsTableRangeAnnotation()
        : DummyTypeAnnotation("ecs_table_range_t", "ecs_table_range_t", sizeof(void*), alignof(void*)) {}
};

MAKE_TYPE_FACTORY(ecs_var_t, ecs_var_t);
struct EcsVarAnnotation : das::DummyTypeAnnotation {
    EcsVarAnnotation()
        : DummyTypeAnnotation("ecs_var_t", "ecs_var_t", sizeof(void*), alignof(void*)) {}
};

// --- Defined structs (ManagedStructureAnnotation) ---

MAKE_TYPE_FACTORY(ecs_header_t, ecs_header_t);
struct EcsHeaderAnnotation : das::ManagedStructureAnnotation<ecs_header_t, false> {
    EcsHeaderAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_header_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(type)>("vtype", "type");
        addField<DAS_BIND_MANAGED_FIELD(refcount)>("refcount", "refcount");
        addField<DAS_BIND_MANAGED_FIELD(mixins)>("mixins", "mixins");
    }
};

MAKE_TYPE_FACTORY(ecs_term_ref_t, ecs_term_ref_t);
struct EcsTermRefAnnotation : das::ManagedStructureAnnotation<ecs_term_ref_t, false> {
    EcsTermRefAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_term_ref_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(id)>("id", "id");
        addField<DAS_BIND_MANAGED_FIELD(name)>("name", "name");
    }
};

MAKE_TYPE_FACTORY(ecs_term_t, ecs_term_t);
struct EcsTermAnnotation : das::ManagedStructureAnnotation<ecs_term_t, false> {
    EcsTermAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_term_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(id)>("id", "id");
        addField<DAS_BIND_MANAGED_FIELD(src)>("src", "src");
        addField<DAS_BIND_MANAGED_FIELD(first)>("first", "first");
        addField<DAS_BIND_MANAGED_FIELD(second)>("second", "second");
        addField<DAS_BIND_MANAGED_FIELD(trav)>("trav", "trav");
        addField<DAS_BIND_MANAGED_FIELD(inout)>("inout", "inout");
        addField<DAS_BIND_MANAGED_FIELD(oper)>("oper", "oper");
        addField<DAS_BIND_MANAGED_FIELD(field_index)>("field_index", "field_index");
    }
};

MAKE_TYPE_FACTORY(ecs_query_t, ecs_query_t);
struct EcsQueryAnnotation : das::ManagedStructureAnnotation<ecs_query_t, false> {
    EcsQueryAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_query_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(hdr)>("hdr", "hdr");
        addField<DAS_BIND_MANAGED_FIELD(terms)>("terms", "terms");
        addField<DAS_BIND_MANAGED_FIELD(sizes)>("sizes", "sizes");
        addField<DAS_BIND_MANAGED_FIELD(ids)>("ids", "ids");
        addField<DAS_BIND_MANAGED_FIELD(bloom_filter)>("bloom_filter", "bloom_filter");
        addField<DAS_BIND_MANAGED_FIELD(flags)>("flags", "flags");
        addField<DAS_BIND_MANAGED_FIELD(var_count)>("var_count", "var_count");
        addField<DAS_BIND_MANAGED_FIELD(term_count)>("term_count", "term_count");
        addField<DAS_BIND_MANAGED_FIELD(field_count)>("field_count", "field_count");
        addField<DAS_BIND_MANAGED_FIELD(fixed_fields)>("fixed_fields", "fixed_fields");
        addField<DAS_BIND_MANAGED_FIELD(var_fields)>("var_fields", "var_fields");
        addField<DAS_BIND_MANAGED_FIELD(static_id_fields)>("static_id_fields", "static_id_fields");
        addField<DAS_BIND_MANAGED_FIELD(data_fields)>("data_fields", "data_fields");
        addField<DAS_BIND_MANAGED_FIELD(write_fields)>("write_fields", "write_fields");
        addField<DAS_BIND_MANAGED_FIELD(read_fields)>("read_fields", "read_fields");
        addField<DAS_BIND_MANAGED_FIELD(row_fields)>("row_fields", "row_fields");
        addField<DAS_BIND_MANAGED_FIELD(shared_readonly_fields)>("shared_readonly_fields", "shared_readonly_fields");
        addField<DAS_BIND_MANAGED_FIELD(set_fields)>("set_fields", "set_fields");
        addField<DAS_BIND_MANAGED_FIELD(vars)>("vars", "vars");
        addField<DAS_BIND_MANAGED_FIELD(ctx)>("ctx", "ctx");
        addField<DAS_BIND_MANAGED_FIELD(binding_ctx)>("binding_ctx", "binding_ctx");
        addField<DAS_BIND_MANAGED_FIELD(entity)>("entity", "entity");
        addField<DAS_BIND_MANAGED_FIELD(real_world)>("real_world", "real_world");
        addField<DAS_BIND_MANAGED_FIELD(world)>("world", "world");
        addField<DAS_BIND_MANAGED_FIELD(eval_count)>("eval_count", "eval_count");
    }
};

MAKE_TYPE_FACTORY(ecs_observer_t, ecs_observer_t);
struct EcsObserverAnnotation : das::ManagedStructureAnnotation<ecs_observer_t, false> {
    EcsObserverAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_observer_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(hdr)>("hdr", "hdr");
        addField<DAS_BIND_MANAGED_FIELD(query)>("query", "query");
        addField<DAS_BIND_MANAGED_FIELD(events)>("events", "events");
        addField<DAS_BIND_MANAGED_FIELD(event_count)>("event_count", "event_count");
        addField<DAS_BIND_MANAGED_FIELD(ctx)>("ctx", "ctx");
        addField<DAS_BIND_MANAGED_FIELD(callback_ctx)>("callback_ctx", "callback_ctx");
        addField<DAS_BIND_MANAGED_FIELD(run_ctx)>("run_ctx", "run_ctx");
        addField<DAS_BIND_MANAGED_FIELD(observable)>("observable", "observable");
        addField<DAS_BIND_MANAGED_FIELD(world)>("world", "world");
        addField<DAS_BIND_MANAGED_FIELD(entity)>("entity", "entity");
    }
};

MAKE_TYPE_FACTORY(ecs_type_hooks_t, ecs_type_hooks_t);
struct EcsTypeHooksAnnotation : das::ManagedStructureAnnotation<ecs_type_hooks_t, false> {
    EcsTypeHooksAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_type_hooks_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(flags)>("flags", "flags");
        addField<DAS_BIND_MANAGED_FIELD(ctx)>("ctx", "ctx");
        addField<DAS_BIND_MANAGED_FIELD(binding_ctx)>("binding_ctx", "binding_ctx");
        addField<DAS_BIND_MANAGED_FIELD(lifecycle_ctx)>("lifecycle_ctx", "lifecycle_ctx");
    }
};

MAKE_TYPE_FACTORY(ecs_type_info_t, ecs_type_info_t);
struct EcsTypeInfoAnnotation : das::ManagedStructureAnnotation<ecs_type_info_t, false> {
    EcsTypeInfoAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_type_info_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(size)>("size", "size");
        addField<DAS_BIND_MANAGED_FIELD(alignment)>("alignment", "alignment");
        addField<DAS_BIND_MANAGED_FIELD(hooks)>("hooks", "hooks");
        addField<DAS_BIND_MANAGED_FIELD(component)>("component", "component");
        addField<DAS_BIND_MANAGED_FIELD(name)>("name", "name");
    }
};

MAKE_TYPE_FACTORY(ecs_value_t, ecs_value_t);
struct EcsValueAnnotation : das::ManagedStructureAnnotation<ecs_value_t, false> {
    EcsValueAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_value_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(type)>("vtype", "type");
        addField<DAS_BIND_MANAGED_FIELD(ptr)>("ptr", "ptr");
    }
};

MAKE_TYPE_FACTORY(ecs_entity_desc_t, ecs_entity_desc_t);
struct EcsEntityDescAnnotation : das::ManagedStructureAnnotation<ecs_entity_desc_t, false> {
    EcsEntityDescAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_entity_desc_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(_canary)>("_canary", "_canary");
        addField<DAS_BIND_MANAGED_FIELD(id)>("id", "id");
        addField<DAS_BIND_MANAGED_FIELD(parent)>("parent", "parent");
        addField<DAS_BIND_MANAGED_FIELD(name)>("name", "name");
        addField<DAS_BIND_MANAGED_FIELD(sep)>("sep", "sep");
        addField<DAS_BIND_MANAGED_FIELD(root_sep)>("root_sep", "root_sep");
        addField<DAS_BIND_MANAGED_FIELD(symbol)>("symbol", "symbol");
        addField<DAS_BIND_MANAGED_FIELD(use_low_id)>("use_low_id", "use_low_id");
        addField<DAS_BIND_MANAGED_FIELD(add)>("add", "add");
        addField<DAS_BIND_MANAGED_FIELD(set)>("set", "set");
        addField<DAS_BIND_MANAGED_FIELD(add_expr)>("add_expr", "add_expr");
    }
};

MAKE_TYPE_FACTORY(ecs_bulk_desc_t, ecs_bulk_desc_t);
struct EcsBulkDescAnnotation : das::ManagedStructureAnnotation<ecs_bulk_desc_t, false> {
    EcsBulkDescAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_bulk_desc_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(_canary)>("_canary", "_canary");
        addField<DAS_BIND_MANAGED_FIELD(entities)>("entities", "entities");
        addField<DAS_BIND_MANAGED_FIELD(count)>("count", "count");
        addField<DAS_BIND_MANAGED_FIELD(ids)>("ids", "ids");
        addField<DAS_BIND_MANAGED_FIELD(data)>("data", "data");
        addField<DAS_BIND_MANAGED_FIELD(table)>("table", "table");
    }
};

MAKE_TYPE_FACTORY(ecs_component_desc_t, ecs_component_desc_t);
struct EcsComponentDescAnnotation : das::ManagedStructureAnnotation<ecs_component_desc_t, false> {
    EcsComponentDescAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_component_desc_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(_canary)>("_canary", "_canary");
        addField<DAS_BIND_MANAGED_FIELD(entity)>("entity", "entity");
        addField<DAS_BIND_MANAGED_FIELD(type)>("vtype", "type");
    }
};

MAKE_TYPE_FACTORY(ecs_iter_t, ecs_iter_t);
struct EcsIterAnnotation : das::ManagedStructureAnnotation<ecs_iter_t, false> {
    EcsIterAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_iter_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(world)>("world", "world");
        addField<DAS_BIND_MANAGED_FIELD(real_world)>("real_world", "real_world");
        addField<DAS_BIND_MANAGED_FIELD(offset)>("offset", "offset");
        addField<DAS_BIND_MANAGED_FIELD(count)>("count", "count");
        addField<DAS_BIND_MANAGED_FIELD(entities)>("entities", "entities");
        addField<DAS_BIND_MANAGED_FIELD(ptrs)>("ptrs", "ptrs");
        addField<DAS_BIND_MANAGED_FIELD(trs)>("trs", "trs");
        addField<DAS_BIND_MANAGED_FIELD(columns)>("columns", "columns");
        addField<DAS_BIND_MANAGED_FIELD(sizes)>("sizes", "sizes");
        addField<DAS_BIND_MANAGED_FIELD(table)>("table", "table");
        addField<DAS_BIND_MANAGED_FIELD(other_table)>("other_table", "other_table");
        addField<DAS_BIND_MANAGED_FIELD(ids)>("ids", "ids");
        addField<DAS_BIND_MANAGED_FIELD(sources)>("sources", "sources");
        addField<DAS_BIND_MANAGED_FIELD(constrained_vars)>("constrained_vars", "constrained_vars");
        addField<DAS_BIND_MANAGED_FIELD(set_fields)>("set_fields", "set_fields");
        addField<DAS_BIND_MANAGED_FIELD(ref_fields)>("ref_fields", "ref_fields");
        addField<DAS_BIND_MANAGED_FIELD(row_fields)>("row_fields", "row_fields");
        addField<DAS_BIND_MANAGED_FIELD(up_fields)>("up_fields", "up_fields");
        addField<DAS_BIND_MANAGED_FIELD(system)>("system", "system");
        addField<DAS_BIND_MANAGED_FIELD(event)>("event", "event");
        addField<DAS_BIND_MANAGED_FIELD(event_id)>("event_id", "event_id");
        addField<DAS_BIND_MANAGED_FIELD(event_cur)>("event_cur", "event_cur");
        addField<DAS_BIND_MANAGED_FIELD(field_count)>("field_count", "field_count");
        addField<DAS_BIND_MANAGED_FIELD(term_index)>("term_index", "term_index");
        addField<DAS_BIND_MANAGED_FIELD(query)>("query", "query");
        addField<DAS_BIND_MANAGED_FIELD(param)>("param", "param");
        addField<DAS_BIND_MANAGED_FIELD(ctx)>("ctx", "ctx");
        addField<DAS_BIND_MANAGED_FIELD(binding_ctx)>("binding_ctx", "binding_ctx");
        addField<DAS_BIND_MANAGED_FIELD(callback_ctx)>("callback_ctx", "callback_ctx");
        addField<DAS_BIND_MANAGED_FIELD(run_ctx)>("run_ctx", "run_ctx");
        addField<DAS_BIND_MANAGED_FIELD(delta_time)>("delta_time", "delta_time");
        addField<DAS_BIND_MANAGED_FIELD(delta_system_time)>("delta_system_time", "delta_system_time");
        addField<DAS_BIND_MANAGED_FIELD(frame_offset)>("frame_offset", "frame_offset");
        addField<DAS_BIND_MANAGED_FIELD(flags)>("flags", "flags");
        addField<DAS_BIND_MANAGED_FIELD(interrupted_by)>("interrupted_by", "interrupted_by");
        addField<DAS_BIND_MANAGED_FIELD(chain_it)>("chain_it", "chain_it");
    }
};

MAKE_TYPE_FACTORY(ecs_query_desc_t, ecs_query_desc_t);
struct EcsQueryDescAnnotation : das::ManagedStructureAnnotation<ecs_query_desc_t, false> {
    EcsQueryDescAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_query_desc_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(_canary)>("_canary", "_canary");
        addField<DAS_BIND_MANAGED_FIELD(terms)>("terms", "terms");
        addField<DAS_BIND_MANAGED_FIELD(expr)>("expr", "expr");
        addField<DAS_BIND_MANAGED_FIELD(flags)>("flags", "flags");
        addField<DAS_BIND_MANAGED_FIELD(order_by)>("order_by", "order_by");
        addField<DAS_BIND_MANAGED_FIELD(group_by)>("group_by", "group_by");
        addField<DAS_BIND_MANAGED_FIELD(group_by_ctx)>("group_by_ctx", "group_by_ctx");
        addField<DAS_BIND_MANAGED_FIELD(ctx)>("ctx", "ctx");
        addField<DAS_BIND_MANAGED_FIELD(binding_ctx)>("binding_ctx", "binding_ctx");
        addField<DAS_BIND_MANAGED_FIELD(entity)>("entity", "entity");
    }
};

MAKE_TYPE_FACTORY(ecs_observer_desc_t, ecs_observer_desc_t);
struct EcsObserverDescAnnotation : das::ManagedStructureAnnotation<ecs_observer_desc_t, false> {
    EcsObserverDescAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_observer_desc_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(_canary)>("_canary", "_canary");
        addField<DAS_BIND_MANAGED_FIELD(entity)>("entity", "entity");
        addField<DAS_BIND_MANAGED_FIELD(query)>("query", "query");
        addField<DAS_BIND_MANAGED_FIELD(events)>("events", "events");
        addField<DAS_BIND_MANAGED_FIELD(yield_existing)>("yield_existing", "yield_existing");
        addField<DAS_BIND_MANAGED_FIELD(global_observer)>("global_observer", "global_observer");
        addField<DAS_BIND_MANAGED_FIELD(ctx)>("ctx", "ctx");
        addField<DAS_BIND_MANAGED_FIELD(callback_ctx)>("callback_ctx", "callback_ctx");
        addField<DAS_BIND_MANAGED_FIELD(run_ctx)>("run_ctx", "run_ctx");
        addField<DAS_BIND_MANAGED_FIELD(last_event_id)>("last_event_id", "last_event_id");
    }
};

MAKE_TYPE_FACTORY(ecs_event_desc_t, ecs_event_desc_t);
struct EcsEventDescAnnotation : das::ManagedStructureAnnotation<ecs_event_desc_t, false> {
    EcsEventDescAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_event_desc_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(event)>("event", "event");
        addField<DAS_BIND_MANAGED_FIELD(table)>("table", "table");
        addField<DAS_BIND_MANAGED_FIELD(other_table)>("other_table", "other_table");
        addField<DAS_BIND_MANAGED_FIELD(offset)>("offset", "offset");
        addField<DAS_BIND_MANAGED_FIELD(count)>("count", "count");
        addField<DAS_BIND_MANAGED_FIELD(entity)>("entity", "entity");
        addField<DAS_BIND_MANAGED_FIELD(param)>("param", "param");
        addField<DAS_BIND_MANAGED_FIELD(const_param)>("const_param", "const_param");
        addField<DAS_BIND_MANAGED_FIELD(flags)>("flags", "flags");
    }
};

MAKE_TYPE_FACTORY(ecs_build_info_t, ecs_build_info_t);
struct EcsBuildInfoAnnotation : das::ManagedStructureAnnotation<ecs_build_info_t, false> {
    EcsBuildInfoAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_build_info_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(compiler)>("compiler", "compiler");
        addField<DAS_BIND_MANAGED_FIELD(addons)>("addons", "addons");
        addField<DAS_BIND_MANAGED_FIELD(flags)>("flags", "flags");
        addField<DAS_BIND_MANAGED_FIELD(version)>("version", "version");
        addField<DAS_BIND_MANAGED_FIELD(version_major)>("version_major", "version_major");
        addField<DAS_BIND_MANAGED_FIELD(version_minor)>("version_minor", "version_minor");
        addField<DAS_BIND_MANAGED_FIELD(version_patch)>("version_patch", "version_patch");
        addField<DAS_BIND_MANAGED_FIELD(debug)>("debug", "debug");
        addField<DAS_BIND_MANAGED_FIELD(sanitize)>("sanitize", "sanitize");
        addField<DAS_BIND_MANAGED_FIELD(perf_trace)>("perf_trace", "perf_trace");
    }
};

MAKE_TYPE_FACTORY(ecs_world_info_t, ecs_world_info_t);
struct EcsWorldInfoAnnotation : das::ManagedStructureAnnotation<ecs_world_info_t, false> {
    EcsWorldInfoAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_world_info_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(last_component_id)>("last_component_id", "last_component_id");
        addField<DAS_BIND_MANAGED_FIELD(delta_time_raw)>("delta_time_raw", "delta_time_raw");
        addField<DAS_BIND_MANAGED_FIELD(delta_time)>("delta_time", "delta_time");
        addField<DAS_BIND_MANAGED_FIELD(time_scale)>("time_scale", "time_scale");
        addField<DAS_BIND_MANAGED_FIELD(target_fps)>("target_fps", "target_fps");
        addField<DAS_BIND_MANAGED_FIELD(frame_time_total)>("frame_time_total", "frame_time_total");
        addField<DAS_BIND_MANAGED_FIELD(system_time_total)>("system_time_total", "system_time_total");
        addField<DAS_BIND_MANAGED_FIELD(emit_time_total)>("emit_time_total", "emit_time_total");
        addField<DAS_BIND_MANAGED_FIELD(merge_time_total)>("merge_time_total", "merge_time_total");
        addField<DAS_BIND_MANAGED_FIELD(rematch_time_total)>("rematch_time_total", "rematch_time_total");
        addField<DAS_BIND_MANAGED_FIELD(world_time_total)>("world_time_total", "world_time_total");
        addField<DAS_BIND_MANAGED_FIELD(world_time_total_raw)>("world_time_total_raw", "world_time_total_raw");
        addField<DAS_BIND_MANAGED_FIELD(frame_count_total)>("frame_count_total", "frame_count_total");
        addField<DAS_BIND_MANAGED_FIELD(merge_count_total)>("merge_count_total", "merge_count_total");
        addField<DAS_BIND_MANAGED_FIELD(eval_comp_monitors_total)>("eval_comp_monitors_total", "eval_comp_monitors_total");
        addField<DAS_BIND_MANAGED_FIELD(rematch_count_total)>("rematch_count_total", "rematch_count_total");
        addField<DAS_BIND_MANAGED_FIELD(id_create_total)>("id_create_total", "id_create_total");
        addField<DAS_BIND_MANAGED_FIELD(id_delete_total)>("id_delete_total", "id_delete_total");
        addField<DAS_BIND_MANAGED_FIELD(table_create_total)>("table_create_total", "table_create_total");
        addField<DAS_BIND_MANAGED_FIELD(table_delete_total)>("table_delete_total", "table_delete_total");
        addField<DAS_BIND_MANAGED_FIELD(pipeline_build_count_total)>("pipeline_build_count_total", "pipeline_build_count_total");
        addField<DAS_BIND_MANAGED_FIELD(systems_ran_total)>("systems_ran_total", "systems_ran_total");
        addField<DAS_BIND_MANAGED_FIELD(observers_ran_total)>("observers_ran_total", "observers_ran_total");
        addField<DAS_BIND_MANAGED_FIELD(queries_ran_total)>("queries_ran_total", "queries_ran_total");
        addField<DAS_BIND_MANAGED_FIELD(tag_id_count)>("tag_id_count", "tag_id_count");
        addField<DAS_BIND_MANAGED_FIELD(component_id_count)>("component_id_count", "component_id_count");
        addField<DAS_BIND_MANAGED_FIELD(pair_id_count)>("pair_id_count", "pair_id_count");
        addField<DAS_BIND_MANAGED_FIELD(table_count)>("table_count", "table_count");
        addField<DAS_BIND_MANAGED_FIELD(creation_time)>("creation_time", "creation_time");
        addField<DAS_BIND_MANAGED_FIELD(name_prefix)>("name_prefix", "name_prefix");
    }
};

MAKE_TYPE_FACTORY(ecs_query_group_info_t, ecs_query_group_info_t);
struct EcsQueryGroupInfoAnnotation : das::ManagedStructureAnnotation<ecs_query_group_info_t, false> {
    EcsQueryGroupInfoAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_query_group_info_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(id)>("id", "id");
        addField<DAS_BIND_MANAGED_FIELD(match_count)>("match_count", "match_count");
        addField<DAS_BIND_MANAGED_FIELD(table_count)>("table_count", "table_count");
        addField<DAS_BIND_MANAGED_FIELD(ctx)>("ctx", "ctx");
    }
};

MAKE_TYPE_FACTORY(ecs_entity_range_t, ecs_entity_range_t);
struct EcsEntityRangeAnnotation : das::ManagedStructureAnnotation<ecs_entity_range_t, false> {
    EcsEntityRangeAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_entity_range_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(min)>("min", "min");
        addField<DAS_BIND_MANAGED_FIELD(max)>("max", "max");
        addField<DAS_BIND_MANAGED_FIELD(cur)>("cur", "cur");
    }
};

MAKE_TYPE_FACTORY(EcsIdentifier, EcsIdentifier);
struct EcsIdentifierAnnotation : das::ManagedStructureAnnotation<EcsIdentifier, false> {
    EcsIdentifierAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("EcsIdentifier", ml) {
        addField<DAS_BIND_MANAGED_FIELD(value)>("value", "value");
        addField<DAS_BIND_MANAGED_FIELD(length)>("length", "length");
        addField<DAS_BIND_MANAGED_FIELD(hash)>("hash", "hash");
        addField<DAS_BIND_MANAGED_FIELD(index_hash)>("index_hash", "index_hash");
    }
};

MAKE_TYPE_FACTORY(EcsComponent, EcsComponent);
struct EcsComponentAnnotation : das::ManagedStructureAnnotation<EcsComponent, false> {
    EcsComponentAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("EcsComponent", ml) {
        addField<DAS_BIND_MANAGED_FIELD(size)>("size", "size");
        addField<DAS_BIND_MANAGED_FIELD(alignment)>("alignment", "alignment");
    }
};

MAKE_TYPE_FACTORY(EcsPoly, EcsPoly);
struct EcsPolyAnnotation : das::ManagedStructureAnnotation<EcsPoly, false> {
    EcsPolyAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("EcsPoly", ml) {
    }
};

MAKE_TYPE_FACTORY(EcsDefaultChildComponent, EcsDefaultChildComponent);
struct EcsDefaultChildComponentAnnotation : das::ManagedStructureAnnotation<EcsDefaultChildComponent, false> {
    EcsDefaultChildComponentAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("EcsDefaultChildComponent", ml) {
        addField<DAS_BIND_MANAGED_FIELD(component)>("component", "component");
    }
};

MAKE_TYPE_FACTORY(EcsParent, EcsParent);
struct EcsParentAnnotation : das::ManagedStructureAnnotation<EcsParent, false> {
    EcsParentAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("EcsParent", ml) {
        addField<DAS_BIND_MANAGED_FIELD(value)>("value", "value");
    }
};

MAKE_TYPE_FACTORY(EcsTreeSpawner, EcsTreeSpawner);
struct EcsTreeSpawnerAnnotation : das::ManagedStructureAnnotation<EcsTreeSpawner, false> {
    EcsTreeSpawnerAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("EcsTreeSpawner", ml) {
    }
};

MAKE_TYPE_FACTORY(ecs_delete_empty_tables_desc_t, ecs_delete_empty_tables_desc_t);
struct EcsDeleteEmptyTablesDescAnnotation : das::ManagedStructureAnnotation<ecs_delete_empty_tables_desc_t, false> {
    EcsDeleteEmptyTablesDescAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_delete_empty_tables_desc_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(clear_generation)>("clear_generation", "clear_generation");
        addField<DAS_BIND_MANAGED_FIELD(delete_generation)>("delete_generation", "delete_generation");
        addField<DAS_BIND_MANAGED_FIELD(time_budget_seconds)>("time_budget_seconds", "time_budget_seconds");
        addField<DAS_BIND_MANAGED_FIELD(offset)>("offset", "offset");
    }
};

MAKE_TYPE_FACTORY(ecs_query_count_t, ecs_query_count_t);
struct EcsQueryCountAnnotation : das::ManagedStructureAnnotation<ecs_query_count_t, false> {
    EcsQueryCountAnnotation(das::ModuleLibrary &ml) : ManagedStructureAnnotation("ecs_query_count_t", ml) {
        addField<DAS_BIND_MANAGED_FIELD(results)>("results", "results");
        addField<DAS_BIND_MANAGED_FIELD(entities)>("entities", "entities");
        addField<DAS_BIND_MANAGED_FIELD(tables)>("tables", "tables");
    }
};
