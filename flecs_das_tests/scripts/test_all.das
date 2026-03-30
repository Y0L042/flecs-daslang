// Main test runner for flecs_das module
require flecs
require test_world
// require test_components
// require test_queries

[export]
def main()
    print("\n╔════════════════════════════════════════╗")
    print("║   Flecs DASLang Module Test Suite   ║")
    print("╚════════════════════════════════════════╝\n")
    
    // Run all test suites
    run_tests()              // World lifecycle tests
    // run_component_tests()    // Component and entity tests
    // run_query_tests()        // Query and iterator tests
    
    print("\n╔════════════════════════════════════════╗")
    print("║        All Test Suites Completed       ║")
    print("╚════════════════════════════════════════╝\n")
