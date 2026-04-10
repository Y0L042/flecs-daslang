SolutionRoot = SolutionRoot or "../../.."

project "flecs_das_tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

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
    }

    libdirs(prependRoot(SolutionRoot, global_libdirs))

    links(global_links)

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
        staticruntime "On"
        runtime "Debug"

    filter "configurations:Release"
        optimize "Speed"
        symbols "Off"
        staticruntime "On"
        runtime "Release"
    filter {}
