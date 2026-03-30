// Test basic world creation and lifecycle
options gen2
require flecs

def test_world_creation() : bool {
    // Create a world
    var world = ecs_init()
    if (world == null) {
        print("ERROR: Failed to create world")
        return false
    }
    
    print("Created world successfully")

    return true
}

[export]
def run_tests() {
    print("\n=== Running Flecs Module Tests ===\n")
    
    if (test_world_creation()) {
        print("\nWorld lifecycle test PASSED\n")
    } else {
        print("\nWorld lifecycle test FAILED\n")
    }
    
    print("=== All Tests Completed ===\n")
}
