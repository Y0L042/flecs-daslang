dofile "configs.lua"

function prependRoot(root, dirs)
    local result = {}
    for _, dir in ipairs(dirs) do
        table.insert(result, root .. "/" .. dir)
    end
    return result
end

workspace "flecs-daslang"
    architecture "x64"
    startproject "flecs_das"

    configurations { "Debug", "Release" }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    staticruntime "On"

    IncludeDir = {}
    IncludeDir["flecs"]   = "flecs_das/vendor/flecs/include"
    IncludeDir["daslang"] = "flecs_das/vendor/daslang/include"

    dofile "flecs_das/vendor/dependencies.lua"

    include "flecs_das"
    include "flecs_das_tests"
