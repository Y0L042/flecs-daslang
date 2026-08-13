// GENERATED - do not edit manually.
// Regenerate with: bash manage.sh gen-flecs-bind

// --- Annotation registrations ---

addAnnotation(new EcsStageAnnotation());
addAnnotation(new EcsTableAnnotation());
addAnnotation(new EcsObservableAnnotation());
addAnnotation(new EcsRefAnnotation());
addAnnotation(new EcsRecordAnnotation());
addAnnotation(new EcsComponentRecordAnnotation());
addAnnotation(new EcsMixinsAnnotation());
addAnnotation(new EcsTableRecordAnnotation());
addAnnotation(new EcsMapAnnotation());
addAnnotation(new EcsStrbufAnnotation());
addAnnotation(new EcsTableRangeAnnotation());
addAnnotation(new EcsVarAnnotation());
addAnnotation(new EcsHeaderAnnotation(lib));
addAnnotation(new EcsTermRefAnnotation(lib));
addAnnotation(new EcsTermAnnotation(lib));
addAnnotation(new EcsQueryAnnotation(lib));
addAnnotation(new EcsObserverAnnotation(lib));
addAnnotation(new EcsTypeHooksAnnotation(lib));
addAnnotation(new EcsTypeInfoAnnotation(lib));
addAnnotation(new EcsValueAnnotation(lib));
addAnnotation(new EcsEntityDescAnnotation(lib));
addAnnotation(new EcsBulkDescAnnotation(lib));
addAnnotation(new EcsComponentDescAnnotation(lib));
addAnnotation(new EcsIterAnnotation(lib));
addAnnotation(new EcsQueryDescAnnotation(lib));
addAnnotation(new EcsObserverDescAnnotation(lib));
addAnnotation(new EcsEventDescAnnotation(lib));
addAnnotation(new EcsBuildInfoAnnotation(lib));
addAnnotation(new EcsWorldInfoAnnotation(lib));
addAnnotation(new EcsQueryGroupInfoAnnotation(lib));
addAnnotation(new EcsEntityRangeAnnotation(lib));
addAnnotation(new EcsIdentifierAnnotation(lib));
addAnnotation(new EcsComponentAnnotation(lib));
addAnnotation(new EcsPolyAnnotation(lib));
addAnnotation(new EcsDefaultChildComponentAnnotation(lib));
addAnnotation(new EcsParentAnnotation(lib));
addAnnotation(new EcsTreeSpawnerAnnotation(lib));
addAnnotation(new EcsDeleteEmptyTablesDescAnnotation(lib));
addAnnotation(new EcsQueryCountAnnotation(lib));

// --- Function registrations ---

addConstant(*this, "ECS_PAIR", (ecs_id_t)ECS_PAIR);
addConstant(*this, "ECS_AUTO_OVERRIDE", (ecs_id_t)ECS_AUTO_OVERRIDE);
addConstant(*this, "ECS_TOGGLE", (ecs_id_t)ECS_TOGGLE);
addConstant(*this, "ECS_VALUE_PAIR", (ecs_id_t)ECS_VALUE_PAIR);
addConstant(*this, "ecs_id_EcsParent", (ecs_entity_t)ecs_id(EcsParent));
addConstant(*this, "ecs_id_EcsTreeSpawner", (ecs_entity_t)ecs_id(EcsTreeSpawner));
addConstant(*this, "EcsParentDepth", (ecs_entity_t)EcsParentDepth);
addConstant(*this, "EcsQuery", (ecs_entity_t)EcsQuery);
addConstant(*this, "EcsObserver", (ecs_entity_t)EcsObserver);
addConstant(*this, "EcsSystem", (ecs_entity_t)EcsSystem);
addConstant(*this, "EcsFlecs", (ecs_entity_t)EcsFlecs);
addConstant(*this, "EcsFlecsCore", (ecs_entity_t)EcsFlecsCore);
addConstant(*this, "EcsWorld", (ecs_entity_t)EcsWorld);
addConstant(*this, "EcsWildcard", (ecs_entity_t)EcsWildcard);
addConstant(*this, "EcsAny", (ecs_entity_t)EcsAny);
addConstant(*this, "EcsThis", (ecs_entity_t)EcsThis);
addConstant(*this, "EcsVariable", (ecs_entity_t)EcsVariable);
addConstant(*this, "EcsTransitive", (ecs_entity_t)EcsTransitive);
addConstant(*this, "EcsReflexive", (ecs_entity_t)EcsReflexive);
addConstant(*this, "EcsFinal", (ecs_entity_t)EcsFinal);
addConstant(*this, "EcsInheritable", (ecs_entity_t)EcsInheritable);
addConstant(*this, "EcsOnInstantiate", (ecs_entity_t)EcsOnInstantiate);
addConstant(*this, "EcsOverride", (ecs_entity_t)EcsOverride);
addConstant(*this, "EcsInherit", (ecs_entity_t)EcsInherit);
addConstant(*this, "EcsDontInherit", (ecs_entity_t)EcsDontInherit);
addConstant(*this, "EcsSymmetric", (ecs_entity_t)EcsSymmetric);
addConstant(*this, "EcsExclusive", (ecs_entity_t)EcsExclusive);
addConstant(*this, "EcsAcyclic", (ecs_entity_t)EcsAcyclic);
addConstant(*this, "EcsTraversable", (ecs_entity_t)EcsTraversable);
addConstant(*this, "EcsWith", (ecs_entity_t)EcsWith);
addConstant(*this, "EcsOneOf", (ecs_entity_t)EcsOneOf);
addConstant(*this, "EcsCanToggle", (ecs_entity_t)EcsCanToggle);
addConstant(*this, "EcsTrait", (ecs_entity_t)EcsTrait);
addConstant(*this, "EcsRelationship", (ecs_entity_t)EcsRelationship);
addConstant(*this, "EcsTarget", (ecs_entity_t)EcsTarget);
addConstant(*this, "EcsPairIsTag", (ecs_entity_t)EcsPairIsTag);
addConstant(*this, "EcsName", (ecs_entity_t)EcsName);
addConstant(*this, "EcsSymbol", (ecs_entity_t)EcsSymbol);
addConstant(*this, "EcsAlias", (ecs_entity_t)EcsAlias);
addConstant(*this, "EcsChildOf", (ecs_entity_t)EcsChildOf);
addConstant(*this, "EcsIsA", (ecs_entity_t)EcsIsA);
addConstant(*this, "EcsDependsOn", (ecs_entity_t)EcsDependsOn);
addConstant(*this, "EcsSlotOf", (ecs_entity_t)EcsSlotOf);
addConstant(*this, "EcsOrderedChildren", (ecs_entity_t)EcsOrderedChildren);
addConstant(*this, "EcsModule", (ecs_entity_t)EcsModule);
addConstant(*this, "EcsPrefab", (ecs_entity_t)EcsPrefab);
addConstant(*this, "EcsDisabled", (ecs_entity_t)EcsDisabled);
addConstant(*this, "EcsNotQueryable", (ecs_entity_t)EcsNotQueryable);
addConstant(*this, "EcsOnAdd", (ecs_entity_t)EcsOnAdd);
addConstant(*this, "EcsOnRemove", (ecs_entity_t)EcsOnRemove);
addConstant(*this, "EcsOnSet", (ecs_entity_t)EcsOnSet);
addConstant(*this, "EcsMonitor", (ecs_entity_t)EcsMonitor);
addConstant(*this, "EcsOnTableCreate", (ecs_entity_t)EcsOnTableCreate);
addConstant(*this, "EcsOnTableDelete", (ecs_entity_t)EcsOnTableDelete);
addConstant(*this, "EcsOnDelete", (ecs_entity_t)EcsOnDelete);
addConstant(*this, "EcsOnDeleteTarget", (ecs_entity_t)EcsOnDeleteTarget);
addConstant(*this, "EcsRemove", (ecs_entity_t)EcsRemove);
addConstant(*this, "EcsDelete", (ecs_entity_t)EcsDelete);
addConstant(*this, "EcsPanic", (ecs_entity_t)EcsPanic);
addConstant(*this, "EcsSingleton", (ecs_entity_t)EcsSingleton);
addConstant(*this, "EcsSparse", (ecs_entity_t)EcsSparse);
addConstant(*this, "EcsDontFragment", (ecs_entity_t)EcsDontFragment);
addConstant(*this, "EcsPredEq", (ecs_entity_t)EcsPredEq);
addConstant(*this, "EcsPredMatch", (ecs_entity_t)EcsPredMatch);
addConstant(*this, "EcsPredLookup", (ecs_entity_t)EcsPredLookup);
addConstant(*this, "EcsScopeOpen", (ecs_entity_t)EcsScopeOpen);
addConstant(*this, "EcsScopeClose", (ecs_entity_t)EcsScopeClose);
addConstant(*this, "EcsEmpty", (ecs_entity_t)EcsEmpty);
addConstant(*this, "EcsOnStart", (ecs_entity_t)EcsOnStart);
addConstant(*this, "EcsPreFrame", (ecs_entity_t)EcsPreFrame);
addConstant(*this, "EcsOnLoad", (ecs_entity_t)EcsOnLoad);
addConstant(*this, "EcsPostLoad", (ecs_entity_t)EcsPostLoad);
addConstant(*this, "EcsPreUpdate", (ecs_entity_t)EcsPreUpdate);
addConstant(*this, "EcsOnUpdate", (ecs_entity_t)EcsOnUpdate);
addConstant(*this, "EcsOnValidate", (ecs_entity_t)EcsOnValidate);
addConstant(*this, "EcsPostUpdate", (ecs_entity_t)EcsPostUpdate);
addConstant(*this, "EcsPreStore", (ecs_entity_t)EcsPreStore);
addConstant(*this, "EcsOnStore", (ecs_entity_t)EcsOnStore);
addConstant(*this, "EcsPostFrame", (ecs_entity_t)EcsPostFrame);
addConstant(*this, "EcsPhase", (ecs_entity_t)EcsPhase);
addConstant(*this, "EcsConstant", (ecs_entity_t)EcsConstant);

das::addExtern<DAS_BIND_FUN(ecs_mini)>(*this, lib, "ecs_mini", das::SideEffects::modifyExternal, "ecs_mini");
das::addExtern<DAS_BIND_FUN(ecs_is_fini)>(*this, lib, "ecs_is_fini", das::SideEffects::modifyExternal, "ecs_is_fini")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_world_get_flags)>(*this, lib, "ecs_world_get_flags", das::SideEffects::modifyExternal, "ecs_world_get_flags")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_frame_begin)>(*this, lib, "ecs_frame_begin", das::SideEffects::modifyExternal, "ecs_frame_begin")
    ->args({"world", "delta_time"});
das::addExtern<DAS_BIND_FUN(ecs_frame_end)>(*this, lib, "ecs_frame_end", das::SideEffects::modifyExternal, "ecs_frame_end")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_quit)>(*this, lib, "ecs_quit", das::SideEffects::modifyExternal, "ecs_quit")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_should_quit)>(*this, lib, "ecs_should_quit", das::SideEffects::modifyExternal, "ecs_should_quit")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_measure_frame_time)>(*this, lib, "ecs_measure_frame_time", das::SideEffects::modifyExternal, "ecs_measure_frame_time")
    ->args({"world", "enable"});
das::addExtern<DAS_BIND_FUN(ecs_measure_system_time)>(*this, lib, "ecs_measure_system_time", das::SideEffects::modifyExternal, "ecs_measure_system_time")
    ->args({"world", "enable"});
das::addExtern<DAS_BIND_FUN(ecs_set_target_fps)>(*this, lib, "ecs_set_target_fps", das::SideEffects::modifyExternal, "ecs_set_target_fps")
    ->args({"world", "fps"});
das::addExtern<DAS_BIND_FUN(ecs_set_default_query_flags)>(*this, lib, "ecs_set_default_query_flags", das::SideEffects::modifyExternal, "ecs_set_default_query_flags")
    ->args({"world", "flags"});
das::addExtern<DAS_BIND_FUN(ecs_readonly_begin)>(*this, lib, "ecs_readonly_begin", das::SideEffects::modifyExternal, "ecs_readonly_begin")
    ->args({"world", "multi_threaded"});
das::addExtern<DAS_BIND_FUN(ecs_readonly_end)>(*this, lib, "ecs_readonly_end", das::SideEffects::modifyExternal, "ecs_readonly_end")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_merge)>(*this, lib, "ecs_merge", das::SideEffects::modifyExternal, "ecs_merge")
    ->args({"stage"});
das::addExtern<DAS_BIND_FUN(ecs_defer_begin)>(*this, lib, "ecs_defer_begin", das::SideEffects::modifyExternal, "ecs_defer_begin")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_defer_end)>(*this, lib, "ecs_defer_end", das::SideEffects::modifyExternal, "ecs_defer_end")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_defer_suspend)>(*this, lib, "ecs_defer_suspend", das::SideEffects::modifyExternal, "ecs_defer_suspend")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_defer_resume)>(*this, lib, "ecs_defer_resume", das::SideEffects::modifyExternal, "ecs_defer_resume")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_is_deferred)>(*this, lib, "ecs_is_deferred", das::SideEffects::modifyExternal, "ecs_is_deferred")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_is_defer_suspended)>(*this, lib, "ecs_is_defer_suspended", das::SideEffects::modifyExternal, "ecs_is_defer_suspended")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_set_stage_count)>(*this, lib, "ecs_set_stage_count", das::SideEffects::modifyExternal, "ecs_set_stage_count")
    ->args({"world", "stages"});
das::addExtern<DAS_BIND_FUN(ecs_get_stage_count)>(*this, lib, "ecs_get_stage_count", das::SideEffects::modifyExternal, "ecs_get_stage_count")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_get_stage)>(*this, lib, "ecs_get_stage", das::SideEffects::modifyExternal, "ecs_get_stage")
    ->args({"world", "stage_id"});
das::addExtern<DAS_BIND_FUN(ecs_stage_is_readonly)>(*this, lib, "ecs_stage_is_readonly", das::SideEffects::modifyExternal, "ecs_stage_is_readonly")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_stage_new)>(*this, lib, "ecs_stage_new", das::SideEffects::modifyExternal, "ecs_stage_new")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_stage_free)>(*this, lib, "ecs_stage_free", das::SideEffects::modifyExternal, "ecs_stage_free")
    ->args({"stage"});
das::addExtern<DAS_BIND_FUN(ecs_stage_get_id)>(*this, lib, "ecs_stage_get_id", das::SideEffects::modifyExternal, "ecs_stage_get_id")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_get_ctx)>(*this, lib, "ecs_get_ctx", das::SideEffects::modifyExternal, "ecs_get_ctx")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_get_binding_ctx)>(*this, lib, "ecs_get_binding_ctx", das::SideEffects::modifyExternal, "ecs_get_binding_ctx")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_get_build_info)>(*this, lib, "ecs_get_build_info", das::SideEffects::modifyExternal, "ecs_get_build_info");
das::addExtern<DAS_BIND_FUN(ecs_get_world_info)>(*this, lib, "ecs_get_world_info", das::SideEffects::modifyExternal, "ecs_get_world_info")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_dim)>(*this, lib, "ecs_dim", das::SideEffects::modifyExternal, "ecs_dim")
    ->args({"world", "entity_count"});
das::addExtern<DAS_BIND_FUN(ecs_shrink)>(*this, lib, "ecs_shrink", das::SideEffects::modifyExternal, "ecs_shrink")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_entity_range_new)>(*this, lib, "ecs_entity_range_new", das::SideEffects::modifyExternal, "ecs_entity_range_new")
    ->args({"world", "min", "max"});
das::addExtern<DAS_BIND_FUN(ecs_entity_range_set)>(*this, lib, "ecs_entity_range_set", das::SideEffects::modifyExternal, "ecs_entity_range_set")
    ->args({"world", "range"});
das::addExtern<DAS_BIND_FUN(ecs_entity_range_get)>(*this, lib, "ecs_entity_range_get", das::SideEffects::modifyExternal, "ecs_entity_range_get")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_get_max_id)>(*this, lib, "ecs_get_max_id", das::SideEffects::modifyExternal, "ecs_get_max_id")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_run_aperiodic)>(*this, lib, "ecs_run_aperiodic", das::SideEffects::modifyExternal, "ecs_run_aperiodic")
    ->args({"world", "flags"});
das::addExtern<DAS_BIND_FUN(ecs_delete_empty_tables)>(*this, lib, "ecs_delete_empty_tables", das::SideEffects::modifyExternal, "ecs_delete_empty_tables")
    ->args({"world", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_get_world)>(*this, lib, "ecs_get_world", das::SideEffects::modifyExternal, "ecs_get_world")
    ->args({"poly"});
das::addExtern<DAS_BIND_FUN(ecs_get_entity)>(*this, lib, "ecs_get_entity", das::SideEffects::modifyExternal, "ecs_get_entity")
    ->args({"poly"});
das::addExtern<DAS_BIND_FUN(flecs_poly_is_)>(*this, lib, "flecs_poly_is_", das::SideEffects::modifyExternal, "flecs_poly_is_")
    ->args({"object", "type"});
das::addExtern<DAS_BIND_FUN(ecs_make_pair)>(*this, lib, "ecs_make_pair", das::SideEffects::modifyExternal, "ecs_make_pair")
    ->args({"first", "second"});
das::addExtern<DAS_BIND_FUN(ecs_exclusive_access_begin)>(*this, lib, "ecs_exclusive_access_begin", das::SideEffects::modifyExternal, "ecs_exclusive_access_begin")
    ->args({"world", "thread_name"});
das::addExtern<DAS_BIND_FUN(ecs_exclusive_access_end)>(*this, lib, "ecs_exclusive_access_end", das::SideEffects::modifyExternal, "ecs_exclusive_access_end")
    ->args({"world", "lock_world"});
das::addExtern<DAS_BIND_FUN(ecs_new)>(*this, lib, "ecs_new", das::SideEffects::modifyExternal, "ecs_new")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_new_low_id)>(*this, lib, "ecs_new_low_id", das::SideEffects::modifyExternal, "ecs_new_low_id")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_new_w_id)>(*this, lib, "ecs_new_w_id", das::SideEffects::modifyExternal, "ecs_new_w_id")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_new_w_table)>(*this, lib, "ecs_new_w_table", das::SideEffects::modifyExternal, "ecs_new_w_table")
    ->args({"world", "table"});
das::addExtern<DAS_BIND_FUN(ecs_entity_init)>(*this, lib, "ecs_entity_init", das::SideEffects::modifyExternal, "ecs_entity_init")
    ->args({"world", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_bulk_init)>(*this, lib, "ecs_bulk_init", das::SideEffects::modifyExternal, "ecs_bulk_init")
    ->args({"world", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_bulk_new_w_id)>(*this, lib, "ecs_bulk_new_w_id", das::SideEffects::modifyExternal, "ecs_bulk_new_w_id")
    ->args({"world", "component", "count"});
das::addExtern<DAS_BIND_FUN(ecs_clone)>(*this, lib, "ecs_clone", das::SideEffects::modifyExternal, "ecs_clone")
    ->args({"world", "dst", "src", "copy_value"});
das::addExtern<DAS_BIND_FUN(ecs_delete)>(*this, lib, "ecs_delete", das::SideEffects::modifyExternal, "ecs_delete")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_delete_with)>(*this, lib, "ecs_delete_with", das::SideEffects::modifyExternal, "ecs_delete_with")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_set_child_order)>(*this, lib, "ecs_set_child_order", das::SideEffects::modifyExternal, "ecs_set_child_order")
    ->args({"world", "parent", "children", "child_count"});
das::addExtern<DAS_BIND_FUN(ecs_get_ordered_children), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ecs_get_ordered_children", das::SideEffects::modifyExternal, "ecs_get_ordered_children")
    ->args({"world", "parent"});
das::addExtern<DAS_BIND_FUN(ecs_add_id)>(*this, lib, "ecs_add_id", das::SideEffects::modifyExternal, "ecs_add_id")
    ->args({"world", "entity", "component"});
das::addExtern<DAS_BIND_FUN(ecs_remove_id)>(*this, lib, "ecs_remove_id", das::SideEffects::modifyExternal, "ecs_remove_id")
    ->args({"world", "entity", "component"});
das::addExtern<DAS_BIND_FUN(ecs_auto_override_id)>(*this, lib, "ecs_auto_override_id", das::SideEffects::modifyExternal, "ecs_auto_override_id")
    ->args({"world", "entity", "component"});
das::addExtern<DAS_BIND_FUN(ecs_clear)>(*this, lib, "ecs_clear", das::SideEffects::modifyExternal, "ecs_clear")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_remove_all)>(*this, lib, "ecs_remove_all", das::SideEffects::modifyExternal, "ecs_remove_all")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_set_with)>(*this, lib, "ecs_set_with", das::SideEffects::modifyExternal, "ecs_set_with")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_get_with)>(*this, lib, "ecs_get_with", das::SideEffects::modifyExternal, "ecs_get_with")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_enable)>(*this, lib, "ecs_enable", das::SideEffects::modifyExternal, "ecs_enable")
    ->args({"world", "entity", "enabled"});
das::addExtern<DAS_BIND_FUN(ecs_enable_id)>(*this, lib, "ecs_enable_id", das::SideEffects::modifyExternal, "ecs_enable_id")
    ->args({"world", "entity", "component", "enable"});
das::addExtern<DAS_BIND_FUN(ecs_is_enabled_id)>(*this, lib, "ecs_is_enabled_id", das::SideEffects::modifyExternal, "ecs_is_enabled_id")
    ->args({"world", "entity", "component"});
das::addExtern<DAS_BIND_FUN(ecs_get_id)>(*this, lib, "ecs_get_id", das::SideEffects::modifyExternal, "ecs_get_id")
    ->args({"world", "entity", "component"});
das::addExtern<DAS_BIND_FUN(ecs_get_mut_id)>(*this, lib, "ecs_get_mut_id", das::SideEffects::modifyExternal, "ecs_get_mut_id")
    ->args({"world", "entity", "component"});
das::addExtern<DAS_BIND_FUN(ecs_ensure_id)>(*this, lib, "ecs_ensure_id", das::SideEffects::modifyExternal, "ecs_ensure_id")
    ->args({"world", "entity", "component", "size"});
// SKIPPED: ecs_ref_init_id (struct-by-value return of opaque type)
das::addExtern<DAS_BIND_FUN(ecs_ref_get_id)>(*this, lib, "ecs_ref_get_id", das::SideEffects::modifyExternal, "ecs_ref_get_id")
    ->args({"world", "ref", "component"});
das::addExtern<DAS_BIND_FUN(ecs_ref_update)>(*this, lib, "ecs_ref_update", das::SideEffects::modifyExternal, "ecs_ref_update")
    ->args({"world", "ref", "component"});
das::addExtern<DAS_BIND_FUN(ecs_emplace_id)>(*this, lib, "ecs_emplace_id", das::SideEffects::modifyExternal, "ecs_emplace_id")
    ->args({"world", "entity", "component", "size", "is_new"});
das::addExtern<DAS_BIND_FUN(ecs_modified_id)>(*this, lib, "ecs_modified_id", das::SideEffects::modifyExternal, "ecs_modified_id")
    ->args({"world", "entity", "component"});
das::addExtern<DAS_BIND_FUN(ecs_set_id)>(*this, lib, "ecs_set_id", das::SideEffects::modifyExternal, "ecs_set_id")
    ->args({"world", "entity", "component", "size", "ptr"});
das::addExtern<DAS_BIND_FUN(ecs_is_valid)>(*this, lib, "ecs_is_valid", das::SideEffects::modifyExternal, "ecs_is_valid")
    ->args({"world", "e"});
das::addExtern<DAS_BIND_FUN(ecs_is_alive)>(*this, lib, "ecs_is_alive", das::SideEffects::modifyExternal, "ecs_is_alive")
    ->args({"world", "e"});
das::addExtern<DAS_BIND_FUN(ecs_strip_generation)>(*this, lib, "ecs_strip_generation", das::SideEffects::modifyExternal, "ecs_strip_generation")
    ->args({"e"});
das::addExtern<DAS_BIND_FUN(ecs_get_alive)>(*this, lib, "ecs_get_alive", das::SideEffects::modifyExternal, "ecs_get_alive")
    ->args({"world", "e"});
das::addExtern<DAS_BIND_FUN(ecs_make_alive)>(*this, lib, "ecs_make_alive", das::SideEffects::modifyExternal, "ecs_make_alive")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_make_alive_id)>(*this, lib, "ecs_make_alive_id", das::SideEffects::modifyExternal, "ecs_make_alive_id")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_exists)>(*this, lib, "ecs_exists", das::SideEffects::modifyExternal, "ecs_exists")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_set_version)>(*this, lib, "ecs_set_version", das::SideEffects::modifyExternal, "ecs_set_version")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_get_version)>(*this, lib, "ecs_get_version", das::SideEffects::modifyExternal, "ecs_get_version")
    ->args({"entity"});
das::addExtern<DAS_BIND_FUN(ecs_get_type)>(*this, lib, "ecs_get_type", das::SideEffects::modifyExternal, "ecs_get_type")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_get_table)>(*this, lib, "ecs_get_table", das::SideEffects::modifyExternal, "ecs_get_table")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_type_str)>(*this, lib, "ecs_type_str", das::SideEffects::modifyExternal, "ecs_type_str")
    ->args({"world", "type"});
das::addExtern<DAS_BIND_FUN(ecs_table_str)>(*this, lib, "ecs_table_str", das::SideEffects::modifyExternal, "ecs_table_str")
    ->args({"world", "table"});
das::addExtern<DAS_BIND_FUN(ecs_entity_str)>(*this, lib, "ecs_entity_str", das::SideEffects::modifyExternal, "ecs_entity_str")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_has_id)>(*this, lib, "ecs_has_id", das::SideEffects::modifyExternal, "ecs_has_id")
    ->args({"world", "entity", "component"});
das::addExtern<DAS_BIND_FUN(ecs_owns_id)>(*this, lib, "ecs_owns_id", das::SideEffects::modifyExternal, "ecs_owns_id")
    ->args({"world", "entity", "component"});
das::addExtern<DAS_BIND_FUN(ecs_get_target)>(*this, lib, "ecs_get_target", das::SideEffects::modifyExternal, "ecs_get_target")
    ->args({"world", "entity", "rel", "index"});
das::addExtern<DAS_BIND_FUN(ecs_get_parent)>(*this, lib, "ecs_get_parent", das::SideEffects::modifyExternal, "ecs_get_parent")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_new_w_parent)>(*this, lib, "ecs_new_w_parent", das::SideEffects::modifyExternal, "ecs_new_w_parent")
    ->args({"world", "parent", "name"});
das::addExtern<DAS_BIND_FUN(ecs_get_target_for_id)>(*this, lib, "ecs_get_target_for_id", das::SideEffects::modifyExternal, "ecs_get_target_for_id")
    ->args({"world", "entity", "rel", "component"});
das::addExtern<DAS_BIND_FUN(ecs_get_depth)>(*this, lib, "ecs_get_depth", das::SideEffects::modifyExternal, "ecs_get_depth")
    ->args({"world", "entity", "rel"});
das::addExtern<DAS_BIND_FUN(ecs_count_id)>(*this, lib, "ecs_count_id", das::SideEffects::modifyExternal, "ecs_count_id")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_get_name)>(*this, lib, "ecs_get_name", das::SideEffects::modifyExternal, "ecs_get_name")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_get_symbol)>(*this, lib, "ecs_get_symbol", das::SideEffects::modifyExternal, "ecs_get_symbol")
    ->args({"world", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_set_name)>(*this, lib, "ecs_set_name", das::SideEffects::modifyExternal, "ecs_set_name")
    ->args({"world", "entity", "name"});
das::addExtern<DAS_BIND_FUN(ecs_set_symbol)>(*this, lib, "ecs_set_symbol", das::SideEffects::modifyExternal, "ecs_set_symbol")
    ->args({"world", "entity", "symbol"});
das::addExtern<DAS_BIND_FUN(ecs_set_alias)>(*this, lib, "ecs_set_alias", das::SideEffects::modifyExternal, "ecs_set_alias")
    ->args({"world", "entity", "alias"});
das::addExtern<DAS_BIND_FUN(ecs_lookup)>(*this, lib, "ecs_lookup", das::SideEffects::modifyExternal, "ecs_lookup")
    ->args({"world", "path"});
das::addExtern<DAS_BIND_FUN(ecs_lookup_child)>(*this, lib, "ecs_lookup_child", das::SideEffects::modifyExternal, "ecs_lookup_child")
    ->args({"world", "parent", "name"});
das::addExtern<DAS_BIND_FUN(ecs_lookup_path_w_sep)>(*this, lib, "ecs_lookup_path_w_sep", das::SideEffects::modifyExternal, "ecs_lookup_path_w_sep")
    ->args({"world", "parent", "path", "sep", "prefix", "recursive"});
das::addExtern<DAS_BIND_FUN(ecs_lookup_symbol)>(*this, lib, "ecs_lookup_symbol", das::SideEffects::modifyExternal, "ecs_lookup_symbol")
    ->args({"world", "symbol", "lookup_as_path", "recursive"});
das::addExtern<DAS_BIND_FUN(ecs_get_path_w_sep)>(*this, lib, "ecs_get_path_w_sep", das::SideEffects::modifyExternal, "ecs_get_path_w_sep")
    ->args({"world", "parent", "child", "sep", "prefix"});
das::addExtern<DAS_BIND_FUN(ecs_get_path_w_sep_buf)>(*this, lib, "ecs_get_path_w_sep_buf", das::SideEffects::modifyExternal, "ecs_get_path_w_sep_buf")
    ->args({"world", "parent", "child", "sep", "prefix", "buf", "escape"});
das::addExtern<DAS_BIND_FUN(ecs_new_from_path_w_sep)>(*this, lib, "ecs_new_from_path_w_sep", das::SideEffects::modifyExternal, "ecs_new_from_path_w_sep")
    ->args({"world", "parent", "path", "sep", "prefix"});
das::addExtern<DAS_BIND_FUN(ecs_add_path_w_sep)>(*this, lib, "ecs_add_path_w_sep", das::SideEffects::modifyExternal, "ecs_add_path_w_sep")
    ->args({"world", "entity", "parent", "path", "sep", "prefix"});
das::addExtern<DAS_BIND_FUN(ecs_set_scope)>(*this, lib, "ecs_set_scope", das::SideEffects::modifyExternal, "ecs_set_scope")
    ->args({"world", "scope"});
das::addExtern<DAS_BIND_FUN(ecs_get_scope)>(*this, lib, "ecs_get_scope", das::SideEffects::modifyExternal, "ecs_get_scope")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_set_name_prefix)>(*this, lib, "ecs_set_name_prefix", das::SideEffects::modifyExternal, "ecs_set_name_prefix")
    ->args({"world", "prefix"});
das::addExtern<DAS_BIND_FUN(ecs_set_lookup_path)>(*this, lib, "ecs_set_lookup_path", das::SideEffects::modifyExternal, "ecs_set_lookup_path")
    ->args({"world", "lookup_path"});
das::addExtern<DAS_BIND_FUN(ecs_get_lookup_path)>(*this, lib, "ecs_get_lookup_path", das::SideEffects::modifyExternal, "ecs_get_lookup_path")
    ->args({"world"});
das::addExtern<DAS_BIND_FUN(ecs_component_init)>(*this, lib, "ecs_component_init", das::SideEffects::modifyExternal, "ecs_component_init")
    ->args({"world", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_get_type_info)>(*this, lib, "ecs_get_type_info", das::SideEffects::modifyExternal, "ecs_get_type_info")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_set_hooks_id)>(*this, lib, "ecs_set_hooks_id", das::SideEffects::modifyExternal, "ecs_set_hooks_id")
    ->args({"world", "component", "hooks"});
das::addExtern<DAS_BIND_FUN(ecs_get_hooks_id)>(*this, lib, "ecs_get_hooks_id", das::SideEffects::modifyExternal, "ecs_get_hooks_id")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_id_is_tag)>(*this, lib, "ecs_id_is_tag", das::SideEffects::modifyExternal, "ecs_id_is_tag")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_id_in_use)>(*this, lib, "ecs_id_in_use", das::SideEffects::modifyExternal, "ecs_id_in_use")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_get_typeid)>(*this, lib, "ecs_get_typeid", das::SideEffects::modifyExternal, "ecs_get_typeid")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_id_match)>(*this, lib, "ecs_id_match", das::SideEffects::modifyExternal, "ecs_id_match")
    ->args({"component", "pattern"});
das::addExtern<DAS_BIND_FUN(ecs_id_is_pair)>(*this, lib, "ecs_id_is_pair", das::SideEffects::modifyExternal, "ecs_id_is_pair")
    ->args({"component"});
das::addExtern<DAS_BIND_FUN(ecs_id_is_wildcard)>(*this, lib, "ecs_id_is_wildcard", das::SideEffects::modifyExternal, "ecs_id_is_wildcard")
    ->args({"component"});
das::addExtern<DAS_BIND_FUN(ecs_id_is_valid)>(*this, lib, "ecs_id_is_valid", das::SideEffects::modifyExternal, "ecs_id_is_valid")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_id_get_flags)>(*this, lib, "ecs_id_get_flags", das::SideEffects::modifyExternal, "ecs_id_get_flags")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_id_flag_str)>(*this, lib, "ecs_id_flag_str", das::SideEffects::modifyExternal, "ecs_id_flag_str")
    ->args({"component_flags"});
das::addExtern<DAS_BIND_FUN(ecs_id_str)>(*this, lib, "ecs_id_str", das::SideEffects::modifyExternal, "ecs_id_str")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_id_str_buf)>(*this, lib, "ecs_id_str_buf", das::SideEffects::modifyExternal, "ecs_id_str_buf")
    ->args({"world", "component", "buf"});
das::addExtern<DAS_BIND_FUN(ecs_id_from_str)>(*this, lib, "ecs_id_from_str", das::SideEffects::modifyExternal, "ecs_id_from_str")
    ->args({"world", "expr"});
das::addExtern<DAS_BIND_FUN(ecs_term_ref_is_set)>(*this, lib, "ecs_term_ref_is_set", das::SideEffects::modifyExternal, "ecs_term_ref_is_set")
    ->args({"ref"});
das::addExtern<DAS_BIND_FUN(ecs_term_is_initialized)>(*this, lib, "ecs_term_is_initialized", das::SideEffects::modifyExternal, "ecs_term_is_initialized")
    ->args({"term"});
das::addExtern<DAS_BIND_FUN(ecs_term_match_this)>(*this, lib, "ecs_term_match_this", das::SideEffects::modifyExternal, "ecs_term_match_this")
    ->args({"term"});
das::addExtern<DAS_BIND_FUN(ecs_term_match_0)>(*this, lib, "ecs_term_match_0", das::SideEffects::modifyExternal, "ecs_term_match_0")
    ->args({"term"});
das::addExtern<DAS_BIND_FUN(ecs_term_str)>(*this, lib, "ecs_term_str", das::SideEffects::modifyExternal, "ecs_term_str")
    ->args({"world", "term"});
das::addExtern<DAS_BIND_FUN(ecs_query_str)>(*this, lib, "ecs_query_str", das::SideEffects::modifyExternal, "ecs_query_str")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_each_id), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ecs_each_id", das::SideEffects::modifyExternal, "ecs_each_id")
    ->args({"world", "component"});
das::addExtern<DAS_BIND_FUN(ecs_each_next)>(*this, lib, "ecs_each_next", das::SideEffects::modifyExternal, "ecs_each_next")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_children), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ecs_children", das::SideEffects::modifyExternal, "ecs_children")
    ->args({"world", "parent"});
das::addExtern<DAS_BIND_FUN(ecs_children_w_rel), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ecs_children_w_rel", das::SideEffects::modifyExternal, "ecs_children_w_rel")
    ->args({"world", "relationship", "parent"});
das::addExtern<DAS_BIND_FUN(ecs_children_next)>(*this, lib, "ecs_children_next", das::SideEffects::modifyExternal, "ecs_children_next")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_query_init)>(*this, lib, "ecs_query_init", das::SideEffects::modifyExternal, "ecs_query_init")
    ->args({"world", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_query_update)>(*this, lib, "ecs_query_update", das::SideEffects::modifyExternal, "ecs_query_update")
    ->args({"world", "entity", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_query_fini)>(*this, lib, "ecs_query_fini", das::SideEffects::modifyExternal, "ecs_query_fini")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_query_find_var)>(*this, lib, "ecs_query_find_var", das::SideEffects::modifyExternal, "ecs_query_find_var")
    ->args({"query", "name"});
das::addExtern<DAS_BIND_FUN(ecs_query_var_name)>(*this, lib, "ecs_query_var_name", das::SideEffects::modifyExternal, "ecs_query_var_name")
    ->args({"query", "var_id"});
das::addExtern<DAS_BIND_FUN(ecs_query_var_is_entity)>(*this, lib, "ecs_query_var_is_entity", das::SideEffects::modifyExternal, "ecs_query_var_is_entity")
    ->args({"query", "var_id"});
das::addExtern<DAS_BIND_FUN(ecs_query_iter), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ecs_query_iter", das::SideEffects::modifyExternal, "ecs_query_iter")
    ->args({"world", "query"});
das::addExtern<DAS_BIND_FUN(ecs_query_next)>(*this, lib, "ecs_query_next", das::SideEffects::modifyExternal, "ecs_query_next")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_query_has)>(*this, lib, "ecs_query_has", das::SideEffects::modifyExternal, "ecs_query_has")
    ->args({"query", "entity", "it"});
das::addExtern<DAS_BIND_FUN(ecs_query_has_table)>(*this, lib, "ecs_query_has_table", das::SideEffects::modifyExternal, "ecs_query_has_table")
    ->args({"query", "table", "it"});
das::addExtern<DAS_BIND_FUN(ecs_query_has_range)>(*this, lib, "ecs_query_has_range", das::SideEffects::modifyExternal, "ecs_query_has_range")
    ->args({"query", "range", "it"});
das::addExtern<DAS_BIND_FUN(ecs_query_match_count)>(*this, lib, "ecs_query_match_count", das::SideEffects::modifyExternal, "ecs_query_match_count")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_query_plan)>(*this, lib, "ecs_query_plan", das::SideEffects::modifyExternal, "ecs_query_plan")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_query_plan_w_profile)>(*this, lib, "ecs_query_plan_w_profile", das::SideEffects::modifyExternal, "ecs_query_plan_w_profile")
    ->args({"query", "it"});
das::addExtern<DAS_BIND_FUN(ecs_query_plans)>(*this, lib, "ecs_query_plans", das::SideEffects::modifyExternal, "ecs_query_plans")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_query_args_parse)>(*this, lib, "ecs_query_args_parse", das::SideEffects::modifyExternal, "ecs_query_args_parse")
    ->args({"query", "it", "expr"});
das::addExtern<DAS_BIND_FUN(ecs_query_changed)>(*this, lib, "ecs_query_changed", das::SideEffects::modifyExternal, "ecs_query_changed")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_query_get)>(*this, lib, "ecs_query_get", das::SideEffects::modifyExternal, "ecs_query_get")
    ->args({"world", "query"});
das::addExtern<DAS_BIND_FUN(ecs_iter_skip)>(*this, lib, "ecs_iter_skip", das::SideEffects::modifyExternal, "ecs_iter_skip")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_set_group)>(*this, lib, "ecs_iter_set_group", das::SideEffects::modifyExternal, "ecs_iter_set_group")
    ->args({"it", "group_id"});
das::addExtern<DAS_BIND_FUN(ecs_query_get_groups)>(*this, lib, "ecs_query_get_groups", das::SideEffects::modifyExternal, "ecs_query_get_groups")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_query_get_group_ctx)>(*this, lib, "ecs_query_get_group_ctx", das::SideEffects::modifyExternal, "ecs_query_get_group_ctx")
    ->args({"query", "group_id"});
das::addExtern<DAS_BIND_FUN(ecs_query_get_group_info)>(*this, lib, "ecs_query_get_group_info", das::SideEffects::modifyExternal, "ecs_query_get_group_info")
    ->args({"query", "group_id"});
das::addExtern<DAS_BIND_FUN(ecs_query_count), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ecs_query_count", das::SideEffects::modifyExternal, "ecs_query_count")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_query_is_true)>(*this, lib, "ecs_query_is_true", das::SideEffects::modifyExternal, "ecs_query_is_true")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_query_get_cache_query)>(*this, lib, "ecs_query_get_cache_query", das::SideEffects::modifyExternal, "ecs_query_get_cache_query")
    ->args({"query"});
das::addExtern<DAS_BIND_FUN(ecs_emit)>(*this, lib, "ecs_emit", das::SideEffects::modifyExternal, "ecs_emit")
    ->args({"world", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_enqueue)>(*this, lib, "ecs_enqueue", das::SideEffects::modifyExternal, "ecs_enqueue")
    ->args({"world", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_observer_init)>(*this, lib, "ecs_observer_init", das::SideEffects::modifyExternal, "ecs_observer_init")
    ->args({"world", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_observer_update)>(*this, lib, "ecs_observer_update", das::SideEffects::modifyExternal, "ecs_observer_update")
    ->args({"world", "observer", "desc"});
das::addExtern<DAS_BIND_FUN(ecs_observer_get)>(*this, lib, "ecs_observer_get", das::SideEffects::modifyExternal, "ecs_observer_get")
    ->args({"world", "observer"});
das::addExtern<DAS_BIND_FUN(ecs_iter_next)>(*this, lib, "ecs_iter_next", das::SideEffects::modifyExternal, "ecs_iter_next")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_fini)>(*this, lib, "ecs_iter_fini", das::SideEffects::modifyExternal, "ecs_iter_fini")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_count)>(*this, lib, "ecs_iter_count", das::SideEffects::modifyExternal, "ecs_iter_count")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_is_true)>(*this, lib, "ecs_iter_is_true", das::SideEffects::modifyExternal, "ecs_iter_is_true")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_first)>(*this, lib, "ecs_iter_first", das::SideEffects::modifyExternal, "ecs_iter_first")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_set_var)>(*this, lib, "ecs_iter_set_var", das::SideEffects::modifyExternal, "ecs_iter_set_var")
    ->args({"it", "var_id", "entity"});
das::addExtern<DAS_BIND_FUN(ecs_iter_set_var_as_table)>(*this, lib, "ecs_iter_set_var_as_table", das::SideEffects::modifyExternal, "ecs_iter_set_var_as_table")
    ->args({"it", "var_id", "table"});
das::addExtern<DAS_BIND_FUN(ecs_iter_set_var_as_range)>(*this, lib, "ecs_iter_set_var_as_range", das::SideEffects::modifyExternal, "ecs_iter_set_var_as_range")
    ->args({"it", "var_id", "range"});
das::addExtern<DAS_BIND_FUN(ecs_iter_get_var)>(*this, lib, "ecs_iter_get_var", das::SideEffects::modifyExternal, "ecs_iter_get_var")
    ->args({"it", "var_id"});
das::addExtern<DAS_BIND_FUN(ecs_iter_get_var_name)>(*this, lib, "ecs_iter_get_var_name", das::SideEffects::modifyExternal, "ecs_iter_get_var_name")
    ->args({"it", "var_id"});
das::addExtern<DAS_BIND_FUN(ecs_iter_get_var_count)>(*this, lib, "ecs_iter_get_var_count", das::SideEffects::modifyExternal, "ecs_iter_get_var_count")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_get_vars)>(*this, lib, "ecs_iter_get_vars", das::SideEffects::modifyExternal, "ecs_iter_get_vars")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_get_var_as_table)>(*this, lib, "ecs_iter_get_var_as_table", das::SideEffects::modifyExternal, "ecs_iter_get_var_as_table")
    ->args({"it", "var_id"});
// SKIPPED: ecs_iter_get_var_as_range (struct-by-value return of opaque type)
das::addExtern<DAS_BIND_FUN(ecs_iter_var_is_constrained)>(*this, lib, "ecs_iter_var_is_constrained", das::SideEffects::modifyExternal, "ecs_iter_var_is_constrained")
    ->args({"it", "var_id"});
das::addExtern<DAS_BIND_FUN(ecs_iter_get_group)>(*this, lib, "ecs_iter_get_group", das::SideEffects::modifyExternal, "ecs_iter_get_group")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_changed)>(*this, lib, "ecs_iter_changed", das::SideEffects::modifyExternal, "ecs_iter_changed")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_iter_str)>(*this, lib, "ecs_iter_str", das::SideEffects::modifyExternal, "ecs_iter_str")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_page_iter), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ecs_page_iter", das::SideEffects::modifyExternal, "ecs_page_iter")
    ->args({"it", "offset", "limit"});
das::addExtern<DAS_BIND_FUN(ecs_page_next)>(*this, lib, "ecs_page_next", das::SideEffects::modifyExternal, "ecs_page_next")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_worker_iter), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "ecs_worker_iter", das::SideEffects::modifyExternal, "ecs_worker_iter")
    ->args({"it", "index", "count"});
das::addExtern<DAS_BIND_FUN(ecs_worker_next)>(*this, lib, "ecs_worker_next", das::SideEffects::modifyExternal, "ecs_worker_next")
    ->args({"it"});
das::addExtern<DAS_BIND_FUN(ecs_field_w_size)>(*this, lib, "ecs_field_w_size", das::SideEffects::modifyExternal, "ecs_field_w_size")
    ->args({"it", "size", "index"});
das::addExtern<DAS_BIND_FUN(ecs_field_at_w_size)>(*this, lib, "ecs_field_at_w_size", das::SideEffects::modifyExternal, "ecs_field_at_w_size")
    ->args({"it", "size", "index", "row"});
das::addExtern<DAS_BIND_FUN(ecs_field_is_readonly)>(*this, lib, "ecs_field_is_readonly", das::SideEffects::modifyExternal, "ecs_field_is_readonly")
    ->args({"it", "index"});
das::addExtern<DAS_BIND_FUN(ecs_field_is_writeonly)>(*this, lib, "ecs_field_is_writeonly", das::SideEffects::modifyExternal, "ecs_field_is_writeonly")
    ->args({"it", "index"});
das::addExtern<DAS_BIND_FUN(ecs_field_is_set)>(*this, lib, "ecs_field_is_set", das::SideEffects::modifyExternal, "ecs_field_is_set")
    ->args({"it", "index"});
das::addExtern<DAS_BIND_FUN(ecs_field_id)>(*this, lib, "ecs_field_id", das::SideEffects::modifyExternal, "ecs_field_id")
    ->args({"it", "index"});
das::addExtern<DAS_BIND_FUN(ecs_field_column)>(*this, lib, "ecs_field_column", das::SideEffects::modifyExternal, "ecs_field_column")
    ->args({"it", "index"});
das::addExtern<DAS_BIND_FUN(ecs_field_src)>(*this, lib, "ecs_field_src", das::SideEffects::modifyExternal, "ecs_field_src")
    ->args({"it", "index"});
das::addExtern<DAS_BIND_FUN(ecs_field_size)>(*this, lib, "ecs_field_size", das::SideEffects::modifyExternal, "ecs_field_size")
    ->args({"it", "index"});
das::addExtern<DAS_BIND_FUN(ecs_field_is_self)>(*this, lib, "ecs_field_is_self", das::SideEffects::modifyExternal, "ecs_field_is_self")
    ->args({"it", "index"});
das::addExtern<DAS_BIND_FUN(ecs_table_get_type)>(*this, lib, "ecs_table_get_type", das::SideEffects::modifyExternal, "ecs_table_get_type")
    ->args({"table"});
das::addExtern<DAS_BIND_FUN(ecs_table_get_type_index)>(*this, lib, "ecs_table_get_type_index", das::SideEffects::modifyExternal, "ecs_table_get_type_index")
    ->args({"world", "table", "component"});
das::addExtern<DAS_BIND_FUN(ecs_table_get_column_index)>(*this, lib, "ecs_table_get_column_index", das::SideEffects::modifyExternal, "ecs_table_get_column_index")
    ->args({"world", "table", "component"});
das::addExtern<DAS_BIND_FUN(ecs_table_column_count)>(*this, lib, "ecs_table_column_count", das::SideEffects::modifyExternal, "ecs_table_column_count")
    ->args({"table"});
das::addExtern<DAS_BIND_FUN(ecs_table_type_to_column_index)>(*this, lib, "ecs_table_type_to_column_index", das::SideEffects::modifyExternal, "ecs_table_type_to_column_index")
    ->args({"table", "index"});
das::addExtern<DAS_BIND_FUN(ecs_table_column_to_type_index)>(*this, lib, "ecs_table_column_to_type_index", das::SideEffects::modifyExternal, "ecs_table_column_to_type_index")
    ->args({"table", "index"});
das::addExtern<DAS_BIND_FUN(ecs_table_get_column)>(*this, lib, "ecs_table_get_column", das::SideEffects::modifyExternal, "ecs_table_get_column")
    ->args({"table", "index", "offset"});
das::addExtern<DAS_BIND_FUN(ecs_table_get_id)>(*this, lib, "ecs_table_get_id", das::SideEffects::modifyExternal, "ecs_table_get_id")
    ->args({"world", "table", "component", "offset"});
das::addExtern<DAS_BIND_FUN(ecs_table_get_column_size)>(*this, lib, "ecs_table_get_column_size", das::SideEffects::modifyExternal, "ecs_table_get_column_size")
    ->args({"table", "index"});
das::addExtern<DAS_BIND_FUN(ecs_table_count)>(*this, lib, "ecs_table_count", das::SideEffects::modifyExternal, "ecs_table_count")
    ->args({"table"});
das::addExtern<DAS_BIND_FUN(ecs_table_size)>(*this, lib, "ecs_table_size", das::SideEffects::modifyExternal, "ecs_table_size")
    ->args({"table"});
das::addExtern<DAS_BIND_FUN(ecs_table_entities)>(*this, lib, "ecs_table_entities", das::SideEffects::modifyExternal, "ecs_table_entities")
    ->args({"table"});
das::addExtern<DAS_BIND_FUN(ecs_table_has_id)>(*this, lib, "ecs_table_has_id", das::SideEffects::modifyExternal, "ecs_table_has_id")
    ->args({"world", "table", "component"});
das::addExtern<DAS_BIND_FUN(ecs_table_get_target)>(*this, lib, "ecs_table_get_target", das::SideEffects::modifyExternal, "ecs_table_get_target")
    ->args({"world", "table", "relationship", "index"});
das::addExtern<DAS_BIND_FUN(ecs_table_get_depth)>(*this, lib, "ecs_table_get_depth", das::SideEffects::modifyExternal, "ecs_table_get_depth")
    ->args({"world", "table", "rel"});
das::addExtern<DAS_BIND_FUN(ecs_table_add_id)>(*this, lib, "ecs_table_add_id", das::SideEffects::modifyExternal, "ecs_table_add_id")
    ->args({"world", "table", "component"});
das::addExtern<DAS_BIND_FUN(ecs_table_find)>(*this, lib, "ecs_table_find", das::SideEffects::modifyExternal, "ecs_table_find")
    ->args({"world", "ids", "id_count"});
das::addExtern<DAS_BIND_FUN(ecs_table_remove_id)>(*this, lib, "ecs_table_remove_id", das::SideEffects::modifyExternal, "ecs_table_remove_id")
    ->args({"world", "table", "component"});
das::addExtern<DAS_BIND_FUN(ecs_table_lock)>(*this, lib, "ecs_table_lock", das::SideEffects::modifyExternal, "ecs_table_lock")
    ->args({"world", "table"});
das::addExtern<DAS_BIND_FUN(ecs_table_unlock)>(*this, lib, "ecs_table_unlock", das::SideEffects::modifyExternal, "ecs_table_unlock")
    ->args({"world", "table"});
das::addExtern<DAS_BIND_FUN(ecs_table_has_flags)>(*this, lib, "ecs_table_has_flags", das::SideEffects::modifyExternal, "ecs_table_has_flags")
    ->args({"table", "flags"});
das::addExtern<DAS_BIND_FUN(ecs_table_has_traversable)>(*this, lib, "ecs_table_has_traversable", das::SideEffects::modifyExternal, "ecs_table_has_traversable")
    ->args({"table"});
das::addExtern<DAS_BIND_FUN(ecs_table_swap_rows)>(*this, lib, "ecs_table_swap_rows", das::SideEffects::modifyExternal, "ecs_table_swap_rows")
    ->args({"world", "table", "row_1", "row_2"});
das::addExtern<DAS_BIND_FUN(ecs_commit)>(*this, lib, "ecs_commit", das::SideEffects::modifyExternal, "ecs_commit")
    ->args({"world", "entity", "record", "table", "added", "removed"});
das::addExtern<DAS_BIND_FUN(ecs_search)>(*this, lib, "ecs_search", das::SideEffects::modifyExternal, "ecs_search")
    ->args({"world", "table", "component", "component_out"});
das::addExtern<DAS_BIND_FUN(ecs_search_offset)>(*this, lib, "ecs_search_offset", das::SideEffects::modifyExternal, "ecs_search_offset")
    ->args({"world", "table", "offset", "component", "component_out"});
das::addExtern<DAS_BIND_FUN(ecs_search_relation)>(*this, lib, "ecs_search_relation", das::SideEffects::modifyExternal, "ecs_search_relation")
    ->args({"world", "table", "offset", "component", "rel", "flags", "tgt_out", "component_out", "tr_out"});
das::addExtern<DAS_BIND_FUN(ecs_search_relation_for_entity)>(*this, lib, "ecs_search_relation_for_entity", das::SideEffects::modifyExternal, "ecs_search_relation_for_entity")
    ->args({"world", "entity", "id", "rel", "self", "cr", "tgt_out", "id_out", "tr_out"});
das::addExtern<DAS_BIND_FUN(ecs_table_clear_entities)>(*this, lib, "ecs_table_clear_entities", das::SideEffects::modifyExternal, "ecs_table_clear_entities")
    ->args({"world", "table"});
das::addExtern<DAS_BIND_FUN(ecs_value_init)>(*this, lib, "ecs_value_init", das::SideEffects::modifyExternal, "ecs_value_init")
    ->args({"world", "type", "ptr"});
das::addExtern<DAS_BIND_FUN(ecs_value_init_w_type_info)>(*this, lib, "ecs_value_init_w_type_info", das::SideEffects::modifyExternal, "ecs_value_init_w_type_info")
    ->args({"world", "ti", "ptr"});
das::addExtern<DAS_BIND_FUN(ecs_value_new)>(*this, lib, "ecs_value_new", das::SideEffects::modifyExternal, "ecs_value_new")
    ->args({"world", "type"});
das::addExtern<DAS_BIND_FUN(ecs_value_fini)>(*this, lib, "ecs_value_fini", das::SideEffects::modifyExternal, "ecs_value_fini")
    ->args({"world", "type", "ptr"});
das::addExtern<DAS_BIND_FUN(ecs_value_free)>(*this, lib, "ecs_value_free", das::SideEffects::modifyExternal, "ecs_value_free")
    ->args({"world", "type", "ptr"});
das::addExtern<DAS_BIND_FUN(ecs_value_copy_w_type_info)>(*this, lib, "ecs_value_copy_w_type_info", das::SideEffects::modifyExternal, "ecs_value_copy_w_type_info")
    ->args({"world", "ti", "dst", "src"});
das::addExtern<DAS_BIND_FUN(ecs_value_copy)>(*this, lib, "ecs_value_copy", das::SideEffects::modifyExternal, "ecs_value_copy")
    ->args({"world", "type", "dst", "src"});
das::addExtern<DAS_BIND_FUN(ecs_value_move_w_type_info)>(*this, lib, "ecs_value_move_w_type_info", das::SideEffects::modifyExternal, "ecs_value_move_w_type_info")
    ->args({"world", "ti", "dst", "src"});
das::addExtern<DAS_BIND_FUN(ecs_value_move)>(*this, lib, "ecs_value_move", das::SideEffects::modifyExternal, "ecs_value_move")
    ->args({"world", "type", "dst", "src"});
das::addExtern<DAS_BIND_FUN(ecs_value_move_ctor_w_type_info)>(*this, lib, "ecs_value_move_ctor_w_type_info", das::SideEffects::modifyExternal, "ecs_value_move_ctor_w_type_info")
    ->args({"world", "ti", "dst", "src"});
das::addExtern<DAS_BIND_FUN(ecs_value_move_ctor)>(*this, lib, "ecs_value_move_ctor", das::SideEffects::modifyExternal, "ecs_value_move_ctor")
    ->args({"world", "type", "dst", "src"});