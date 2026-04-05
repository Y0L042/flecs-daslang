local projectRoot = path.getabsolute(".")
package.path = package.path .. ";" .. projectRoot .. "/vendor/?.lua"

newoption {
    trigger     = "deps",
    description = "Process dependencies (update or rebuild based on dependency-specific flags)"
}

SolutionRoot = ".."
ProjectRoot = "."

-- Vendor path overrides: consuming workspaces can set these before including this file
-- to redirect flecs_das to use their own pre-built copies of flecs and daslang.
FLECS_DAS_FLECS_INCLUDE   = FLECS_DAS_FLECS_INCLUDE   or "%{wks.location}/flecs_das/vendor/flecs/include"
FLECS_DAS_DASLANG_INCLUDE = FLECS_DAS_DASLANG_INCLUDE or "%{wks.location}/flecs_das/vendor/daslang/include"
FLECS_DAS_FLECS_LIBDIR    = FLECS_DAS_FLECS_LIBDIR    or ("%{wks.location}/flecs_das/vendor/flecs/" .. VISUAL_STUDIO_BUILD_DIR .. "/Debug")
FLECS_DAS_FLECS_LIBDIR_REL = FLECS_DAS_FLECS_LIBDIR_REL or ("%{wks.location}/flecs_das/vendor/flecs/" .. VISUAL_STUDIO_BUILD_DIR .. "/Release")
FLECS_DAS_DASLANG_LIBDIR  = FLECS_DAS_DASLANG_LIBDIR  or "%{wks.location}/flecs_das/vendor/daslang/lib/Debug"
FLECS_DAS_DASLANG_LIBDIR_REL = FLECS_DAS_DASLANG_LIBDIR_REL or "%{wks.location}/flecs_das/vendor/daslang/lib/Release"

project "flecs_das"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/flecs_das/bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("%{wks.location}/flecs_das/bin-int/" .. outputdir .. "/%{prj.name}")

    files { "src/**.h", "src/**.cpp", "src/**.c", "src/**.hpp", "src/**.inc" }

    includedirs { "src" }

    externalincludedirs {
        FLECS_DAS_FLECS_INCLUDE,
        FLECS_DAS_DASLANG_INCLUDE,
    }

    defines {
        "flecs_STATIC" ,
        "DAS_SMART_PTR_DEBUG=1",
        "DAS_ENABLE_DYN_INCLUDES=1",
        "DAS_ENABLE_EXCEPTIONS=1",
    }

    libdirs {
        FLECS_DAS_FLECS_LIBDIR,
        FLECS_DAS_DASLANG_LIBDIR,
    }

    links {
        "flecs_static.lib",
        "libDaScript.lib",
        "libUriParser.lib",
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

    filter "configurations:Release"
        optimize "Speed"
        symbols "Off"
        libdirs {
            FLECS_DAS_FLECS_LIBDIR_REL,
            FLECS_DAS_DASLANG_LIBDIR_REL,
        }


if _OPTIONS["deps"] then
    local vendorDeps = require "dependencies"
    local allDeps = vendorDeps.getDependencyNames()
    local flaggedDeps = {}

    for _, dep in ipairs(allDeps) do
        if _OPTIONS[dep] then
            table.insert(flaggedDeps, dep)
        end
    end

    local answer
    repeat
        if #flaggedDeps > 0 then
            io.write("This will fetch and rebuild the following dependencies:\n")
            for _, dep in ipairs(flaggedDeps) do
                io.write("  - " .. dep .. "\n")
            end
        else
            io.write("This will fetch and rebuild ALL dependencies:\n")
            for _, dep in ipairs(allDeps) do
                io.write("  - " .. dep .. "\n")
            end
            io.write("\nThis may take a long time.\n")
        end
        io.write("\nContinue with this operation (yes/n)? ")
        io.flush()
        answer=io.read()
    until answer=="yes" or answer=="n"

    if answer == "yes" then
        print("Operation continued.")
        vendorDeps.processDependencies()
    elseif answer == "n" then
        print("Operation aborted.")
    end
end
