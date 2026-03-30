project "flecs_das_tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/tests/bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("%{wks.location}/tests/bin-int/" .. outputdir .. "/%{prj.name}")

    files { "src/**.h", "src/**.cpp" }

    includedirs {
        "src",
        "%{wks.location}/flecs_das/src",
    }

    externalincludedirs {
        "%{wks.location}/flecs_das/vendor/flecs/include",
        "%{wks.location}/flecs_das/vendor/daslang/include",
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
        "%{wks.location}/flecs_das/vendor/flecs/build_vs2026/Debug",
        "%{wks.location}/flecs_das/vendor/daslang/lib/Debug",
    }

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"
        links {
            "libDaScript.lib",
            "libUriParser.lib",
        }
        libdirs {
            "%{wks.location}/flecs_das/vendor/flecs/build_vs2026/Debug",
            "%{wks.location}/flecs_das/vendor/daslang/lib/Debug",
        }

    filter "configurations:Release"
        optimize "Speed"
        symbols "Off"
        links {
            "libDaScript.lib",
            "libUriParser.lib",
        }
        libdirs {
            "%{wks.location}/flecs_das/vendor/flecs/build_vs2026/Release",
            "%{wks.location}/flecs_das/vendor/daslang/lib/Release",
        }
