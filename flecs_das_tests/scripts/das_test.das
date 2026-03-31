// das_test: debug logging and test framework module for daslang
//
// Usage:
//   require das_test
//
//   [export]
//   def run_tests() {
//       begin_suite("My Suite")
//       begin_case("something works")
//       CHECK(1 + 1 == 2, "basic arithmetic")
//       end_case()
//       end_suite()
//   }
//
// Note: This module is intentionally NOT declared 'shared'. Shared modules
// compile into a single context instance across all scripts, which would
// mix counters between test files. Each script gets its own module globals.

options gen2
options no_global_variables = false

module das_test

// ---------------------------------------------------------------------------
// ANSI color codes
// ---------------------------------------------------------------------------

let CLR_RESET  = "\x1B[0m"
let CLR_RED    = "\x1B[31m"
let CLR_GREEN  = "\x1B[32m"
let CLR_YELLOW = "\x1B[33m"
let CLR_CYAN   = "\x1B[36m"
let CLR_BOLD   = "\x1B[1m"
let CLR_DIM    = "\x1B[2m"

// ---------------------------------------------------------------------------
// Global test state
// ---------------------------------------------------------------------------

var g_suite_name   : string = ""
var g_case_name    : string = ""
var g_pass_count   : int    = 0
var g_fail_count   : int    = 0
var g_case_pass    : bool   = true
var g_suite_active : bool   = false
var g_case_active  : bool   = false

// ---------------------------------------------------------------------------
// Colored logging  (independent of test state, usable anywhere)
// ---------------------------------------------------------------------------

def info(msg : string) {
    print("{CLR_CYAN}[INFO ]{CLR_RESET} {msg}\n")
}

def warn(msg : string) {
    print("{CLR_YELLOW}[WARN ]{CLR_RESET} {msg}\n")
}

def err(msg : string) {
    print("{CLR_RED}[ERR  ]{CLR_RESET} {msg}\n")
}

def trace(msg : string) {
    print("{CLR_DIM}[TRACE]{CLR_RESET} {msg}\n")
}

// ---------------------------------------------------------------------------
// Suite lifecycle
// ---------------------------------------------------------------------------

def begin_suite(name : string) {
    g_suite_name   = name
    g_pass_count   = 0
    g_fail_count   = 0
    g_suite_active = true
    g_case_active  = false
    print("{CLR_BOLD}=== Suite: {name} ==={CLR_RESET}\n")
}

def end_suite() {
    if (!g_suite_active) {
        panic("end_suite called without a matching begin_suite")
    }
    g_suite_active = false
    let total = g_pass_count + g_fail_count
    if (g_fail_count == 0) {
        print("{CLR_GREEN}[ OK ]{CLR_RESET} {g_suite_name}: {g_pass_count}/{total} cases passed\n\n")
    } else {
        print("{CLR_RED}[FAIL]{CLR_RESET} {g_suite_name}: {g_fail_count}/{total} cases failed\n\n")
        panic("Suite '{g_suite_name}' failed with {g_fail_count} failure(s)")
    }
}

// ---------------------------------------------------------------------------
// Case lifecycle
// ---------------------------------------------------------------------------

def begin_case(name : string) {
    g_case_name   = name
    g_case_pass   = true
    g_case_active = true
    print("  {CLR_DIM}[ RUN  ]{CLR_RESET} {name}\n")
}

def end_case() {
    if (!g_case_active) {
        panic("end_case called without a matching begin_case")
    }
    g_case_active = false
    if (g_case_pass) {
        g_pass_count++
        print("  {CLR_GREEN}[ PASS ]{CLR_RESET} {g_case_name}\n")
    } else {
        g_fail_count++
        print("  {CLR_RED}[ FAIL ]{CLR_RESET} {g_case_name}\n")
    }
}

// ---------------------------------------------------------------------------
// Check functions  (must be called inside begin_case / end_case)
// ---------------------------------------------------------------------------

// Soft fail: marks the current case as failed and continues execution.
def CHECK(cond : bool; msg : string) {
    if (!cond) {
        g_case_pass = false
        print("    {CLR_RED}FAIL{CLR_RESET} CHECK: {msg}\n")
    }
}

// Soft fail: expects the condition to be false (e.g. an error must NOT have occurred).
def CHECK_FAIL(cond : bool; msg : string) {
    if (cond) {
        g_case_pass = false
        print("    {CLR_RED}FAIL{CLR_RESET} CHECK_FAIL (expected false): {msg}\n")
    }
}

// Hard fail: increments the failure counter directly (because end_case / end_suite
// will never run after the panic unwinds the stack) then panics immediately.
def REQUIRE(cond : bool; msg : string) {
    if (!cond) {
        g_case_pass   = false
        g_case_active = false   // prevent double-counting if end_case is somehow reached
        g_fail_count++
        print("    {CLR_RED}FAIL{CLR_RESET} REQUIRE: {msg}\n")
        panic("REQUIRE failed in case '{g_case_name}': {msg}")
    }
}

// Informational message printed inside a case, no effect on pass/fail.
def MESSAGE(msg : string) {
    print("    {CLR_DIM}MSG  {CLR_RESET} {msg}\n")
}

// ---------------------------------------------------------------------------
// Unconditional assert  (fatal, independent of suite/case state)
// ---------------------------------------------------------------------------

def DAS_ASSERT(cond : bool; msg : string) {
    if (!cond) {
        print("{CLR_RED}ASSERT FAILED{CLR_RESET}: {msg}\n")
        panic("Assertion failed: {msg}")
    }
}
