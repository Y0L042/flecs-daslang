local projectRoot = path.getabsolute(".")
package.path = package.path .. ";" .. projectRoot .. "/vendor/?.lua"

newoption {
    trigger     = "deps",
    description = "Process dependencies (update or rebuild based on dependency-specific flags)"
}

SolutionRoot = ".."
ProjectRoot = "."

project "flecs_das"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/flecs_das/bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("%{wks.location}/flecs_das/bin-int/" .. outputdir .. "/%{prj.name}")

    files { "src/**.h", "src/**.cpp", "src/**.c", "src/**.hpp", "src/**.inc" }

    includedirs { "src" }

    -- Use workspace-location-aware paths for external includes
    externalincludedirs {
        "%{wks.location}/flecs_das/vendor/flecs/include",
        "%{wks.location}/flecs_das/vendor/daslang/include",
    }

    defines { 
        "flecs_STATIC" ,        
        "DAS_SMART_PTR_DEBUG=1",
        "DAS_ENABLE_DYN_INCLUDES=1",
        "DAS_ENABLE_EXCEPTIONS=1",
    }

    libdirs {
        "%{wks.location}/flecs_das/vendor/flecs/" .. VISUAL_STUDIO_BUILD_DIR .. "/Debug",
        "%{wks.location}/flecs_das/vendor/daslang/lib/Debug",
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
        }

    filter "configurations:Release"
        optimize "Speed"
        symbols "Off"
        libdirs {
            "%{wks.location}/flecs_das/vendor/flecs/" .. VISUAL_STUDIO_BUILD_DIR .. "/Release",
            "%{wks.location}/flecs_das/vendor/daslang/lib/Release",
        }
        links {
            "flecs_static.lib",
            "libDaScript.lib",
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
