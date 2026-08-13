#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include <daScript/daScript.h>
#include "../../flecs_das/src/flecs_das.h" //../../flecs_das/flecs_das.h"

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
    fileAccess->introduceNativeModules();

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

static std::string GetDaslangRoot()
{
    std::string file_path(__FILE__);
    // __FILE__ is something like: .../Duin/vendor/flecs-daslang/flecs_das_tests/src/main.cpp
    // daslang root sits at:       .../Duin/vendor/daslang
    size_t pos = file_path.find("vendor");
    if (pos != std::string::npos)
        return file_path.substr(0, pos) + "vendor/daslang";
    return ".";
}

int main(int argc, char *argv[])
{
    std::cout << "flecs_das_tests - Testing the flecs daScript module\n\n";

    das::setDasRoot(GetDaslangRoot());

    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_flecs);
    das::Module::Initialize();

    das::ModuleGroup libGroup;

    const std::string scriptsDir = GetScriptsDir();

    // test_all.das aggregates every suite (it `require`s each test_*.das and
    // calls its run_*_tests entry point), so running it covers them all.
    // It also invokes sandbox() at the end.
    //
    // Failures do surface here: end_suite() panics when a suite has any failed
    // case, and RunDasScript catches that via evalWithCatch and returns false,
    // so a failing CHECK/REQUIRE yields a non-zero exit code.
    int failed = 0;
    std::cout << "[ RUN ] test_all\n";
    if (RunDasScript(libGroup, scriptsDir + "test_all.das", "main"))
    {
        std::cout << "[ PASS ] test_all\n\n";
    }
    else
    {
        std::cout << "[ FAIL ] test_all\n\n";
        ++failed;
    }

    das::Module::Shutdown();

    return failed == 0 ? 0 : 1;
}
