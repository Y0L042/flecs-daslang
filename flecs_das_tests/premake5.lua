project "flecs_das_tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

local flecsDasRoot = "%{wks.location}/flecs_das"

    targetdir ("%{wks.location}/tests/bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("%{wks.location}/tests/bin-int/" .. outputdir .. "/%{prj.name}")

    files { "src/**.h", "src/**.cpp" }

    includedirs {
        "src",
        flecsDasRoot .. "/src",
    }

    externalincludedirs {
        FLECS_DAS_FLECS_INCLUDE,
        FLECS_DAS_DASLANG_INCLUDE,
    }

    defines {
        "flecs_STATIC",
        "DAS_SMART_PTR_DEBUG=1",
        "DAS_ENABLE_DYN_INCLUDES=1",
        "DAS_ENABLE_EXCEPTIONS=1",
    }

    links {
        "flecs_das",
    }

    libdirs {
        "%{wks.location}/flecs_das/bin/" .. outputdir .. "/flecs_das",
        FLECS_DAS_FLECS_LIBDIR,
        FLECS_DAS_DASLANG_LIBDIR,
    }

    filter "action:vs*"
        buildoptions { 
            "/utf-8", 
            '/Zc:__cplusplus', 
            '/Zc:preprocessor' ,
            '/bigobj'
        }  -- Changed: Added /utf-8 flag for Unicode support
        multiprocessorcompile "On"
    filter {}

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"
        links {
            "flecs_static.lib",
            "libDaScript.lib",
            "libUriParser.lib",
        }
        libdirs {
            FLECS_DAS_FLECS_LIBDIR,
            FLECS_DAS_DASLANG_LIBDIR,
        }

    filter "configurations:Release"
        optimize "Speed"
        symbols "Off"
        links {
            "flecs_static.lib",
            "libDaScript.lib",
            "libUriParser.lib",
        }
        libdirs {
            FLECS_DAS_FLECS_LIBDIR_REL,
            FLECS_DAS_DASLANG_LIBDIR_REL,
        }
