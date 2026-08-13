SolutionRoot = SolutionRoot or "../../.."

project "flecs_das_tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    dependson { "flecs_das" }

    local flecsDasRoot = path.join("%{prj.location}", "..", "flecs_das")

    targetdir ("%{wks.location}/Duin/vendor/flecs-daslang/flecs_das_tests/bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("%{wks.location}/Duin/vendor/flecs-daslang/flecs_das_tests/bin-int/" .. outputdir .. "/%{prj.name}")

    files { "src/**.h", "src/**.cpp" }

    includedirs {
        "src",
        flecsDasRoot .. "/src",
    }

    externalincludedirs {
        FLECS_DAS_FLECS_INCLUDE,
        FLECS_DAS_DASLANG_INCLUDE,
    }

    defines(global_defines)
    defines {
        "DAS_SMART_PTR_DEBUG=1",
        "DAS_ENABLE_EXCEPTIONS=1",
        "DAS_ENABLE_DLL=1",
        "flecs_STATIC",
    }

    libdirs {
        path.join("%{wks.location}", "Duin/vendor/flecs-daslang/flecs_das/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/flecs_das"),
        path.join("%{wks.location}", "Duin/vendor/daslang/lib/Debug"),
        path.join("%{wks.location}", "Duin/vendor/flecs/build_vs2026/Debug"),
    }

    links {
        "flecs_das.lib",
        "flecs_static.lib",
        "libDaScriptDyn.lib",
        "libDaScriptDyn_runtime.lib",
    }

    postbuildcommands {
        '{COPYFILE} "%{wks.location}/Duin/vendor/flecs-daslang/flecs_das/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/flecs_das/flecs_das.dll" "%{cfg.targetdir}/flecs_das.dll"',
        '{COPYFILE} "%{wks.location}/Duin/vendor/daslang/bin/Debug/libDaScriptDyn.dll" "%{cfg.targetdir}/libDaScriptDyn.dll"',
        '{COPYFILE} "%{wks.location}/Duin/vendor/daslang/bin/Debug/libDaScriptDyn_runtime.dll" "%{cfg.targetdir}/libDaScriptDyn_runtime.dll"',
    }

    filter "action:vs*"
        buildoptions {
            "/utf-8",
            '/Zc:__cplusplus',
            '/Zc:preprocessor',
            '/bigobj',
        }
        multiprocessorcompile "On"
    filter {}

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"
        staticruntime "Off"
        runtime "Debug"

    filter "configurations:Release"
        optimize "Speed"
        symbols "Off"
        staticruntime "Off"
        runtime "Release"
    filter {}
