dofile "configs.lua"

function prependRoot(root, dirs)
    local result = {}
    for _, dir in ipairs(dirs) do
        table.insert(result, root .. "/" .. dir)
    end
    return result
end

local workspaceRoot = path.getabsolute(".")
local flecsDasRoot = workspaceRoot .. "/flecs_das"

FLECS_DAS_FLECS_INCLUDE = FLECS_DAS_FLECS_INCLUDE or (flecsDasRoot .. "/vendor/flecs/include")
FLECS_DAS_DASLANG_INCLUDE = FLECS_DAS_DASLANG_INCLUDE or (flecsDasRoot .. "/vendor/daslang/include")
FLECS_DAS_FLECS_LIBDIR = FLECS_DAS_FLECS_LIBDIR or (flecsDasRoot .. "/vendor/flecs/" .. VISUAL_STUDIO_BUILD_DIR .. "/Debug")
FLECS_DAS_FLECS_LIBDIR_REL = FLECS_DAS_FLECS_LIBDIR_REL or (flecsDasRoot .. "/vendor/flecs/" .. VISUAL_STUDIO_BUILD_DIR .. "/Release")
FLECS_DAS_DASLANG_LIBDIR = FLECS_DAS_DASLANG_LIBDIR or (flecsDasRoot .. "/vendor/daslang/lib/Debug")
FLECS_DAS_DASLANG_LIBDIR_REL = FLECS_DAS_DASLANG_LIBDIR_REL or (flecsDasRoot .. "/vendor/daslang/lib/Release")

workspace "flecs-daslang"
    architecture "x64"
    startproject "flecs_das"

    configurations { "Debug", "Release" }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    staticruntime "On"

    IncludeDir = {}
    IncludeDir["flecs"]   = FLECS_DAS_FLECS_INCLUDE
    IncludeDir["daslang"] = FLECS_DAS_DASLANG_INCLUDE

    dofile "flecs_das/vendor/dependencies.lua"

    include "flecs_das"
    include "flecs_das_tests"
