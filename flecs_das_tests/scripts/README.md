# Flecs DASLang Module Tests

This directory contains DASLang test scripts for the `flecs_das` module binding.

## Test Files

### test_world.das
Tests basic world lifecycle operations:
- `test_world_creation()`: Tests world creation, finalization, and state checks
- `test_entity_creation()`: Tests entity creation, deletion, and alive checks
- `test_frame_control()`: Tests frame timing operations
- `run_tests()`: Main test runner for world/entity tests

### test_components.das
Tests component registration and entity-component interactions:
- `test_component_registration()`: Tests component registration with descriptors
- `test_entity_component_operations()`: Tests adding/removing components from entities
- `test_entity_naming()`: Tests entity naming and lookup operations
- `run_component_tests()`: Main test runner for component tests

### test_queries.das
Tests query API and iteration:
- `test_query_creation()`: Tests query descriptor creation
- `test_iterator_operations()`: Tests iterator operations
- `run_query_tests()`: Main test runner for query tests

### test_all.das
Main test runner that executes all test suites.

## Running Tests

### From C++
The test can be run from the C++ test executable (`flecs_das_tests.exe`). The main.cpp initializes daScript and loads the flecs module.

### Notes
- Tests use the daScript `print()` function for output
- Tests use exported functions (`[export]` annotation) as entry points
- All tests follow a consistent pattern: create world, run test logic, finalize world
- Component tests use simple component IDs for illustration; full tests would register proper components
- Query tests demonstrate basic query API usage; advanced filtering would require component registration

## Future Enhancements
- Add performance benchmarking tests
- Add stress tests with large entity counts
- Add multi-world tests
- Add error handling and edge case tests
- Add integration tests with compiled daScript modules
