#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include <daScript/daScript.h>
#include <flecs_das.h>

std::string GetScriptsDir()
{
    std::string file_path(__FILE__);
    size_t pos = file_path.find("flecs_das_tests");
    if (pos != std::string::npos)
    {
        return file_path.substr(0, pos) + "flecs_das_tests/scripts/";
    }
    return "scripts/";
}

std::string SafeErrorReport(const das::Error &err)
{
    std::ostringstream oss;
    oss << "\n";
    oss << "What:  " << err.what << "\n";
    oss << "Extra: " << err.extra << "\n";
    oss << "Fixme: " << err.fixme << "\n";
    if (err.at.fileInfo != nullptr)
    {
        try
        {
            oss << "At: " << err.at.describe() << "\n";
        }
        catch (...)
        {
            oss << "At: <describe() threw>\n";
        }
    }
    else
    {
        oss << "At: " << err.at.line << ":" << err.at.column << " <no file>\n";
    }
    oss << "Cerr: " << (int)err.cerr << "\n";
    return oss.str();
}

bool RunDasScript(das::ModuleGroup &libGroup, const std::string &scriptPath, const std::string &exportedFn)
{
    das::TextPrinter tout;

    auto fileAccess = das::make_smart<das::FsFileAccess>();

    das::CodeOfPolicies policies;
    policies.rtti = true;

    std::cout << "  Compiling " << scriptPath << " ...\n";
    das::ProgramPtr program = das::compileDaScript(scriptPath, fileAccess, tout, libGroup, policies);
    if (program->failed())
    {
        std::cerr << "  Compilation FAILED:\n";
        for (auto &err : program->errors)
            std::cerr << SafeErrorReport(err);
        return false;
    }

    das::Context context(program->getContextStackSize());
    if (!program->simulate(context, tout))
    {
        std::cerr << "  Simulation FAILED:\n";
        for (auto &err : program->errors)
            std::cerr << SafeErrorReport(err);
        return false;
    }

    das::SimFunction *fn = context.findFunction(exportedFn.c_str());
    if (!fn)
    {
        std::cerr << "  ERROR: exported function '" << exportedFn << "' not found.\n";
        return false;
    }

    context.evalWithCatch(fn, nullptr, nullptr);
    if (auto ex = context.getException())
    {
        std::cerr << "  Script exception: " << ex << "\n";
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    std::cout << "flecs_das_tests - Testing the flecs daScript module\n\n";

    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_flecs);
    das::Module::Initialize();

    das::ModuleGroup libGroup;

    const std::string scriptsDir = GetScriptsDir();

    struct TestCase
    {
        std::string file;
        std::string fn;
    };
    TestCase tests[] = {
        {scriptsDir + "test_world.das", "run_tests"},
        {scriptsDir + "test_components.das", "run_component_tests"},
        {scriptsDir + "test_queries.das", "run_query_tests"},
    };

    int passed = 0, failed = 0;
    for (auto &tc : tests)
    {
        std::cout << "[ RUN ] " << tc.fn << "\n";
        if (RunDasScript(libGroup, tc.file, tc.fn))
        {
            std::cout << "[ PASS ] " << tc.fn << "\n\n";
            ++passed;
        }
        else
        {
            std::cout << "[ FAIL ] " << tc.fn << "\n\n";
            ++failed;
        }
    }

    das::Module::Shutdown();

    std::cout << "Results: " << passed << " passed, " << failed << " failed.\n";
    return failed == 0 ? 0 : 1;
}
