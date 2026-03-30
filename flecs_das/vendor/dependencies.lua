local vendorDeps = {}
local utils = require("utils")
local VENDOR_DIR = _SCRIPT_DIR  -- absolute path to this script's directory (flecs_das/vendor)

local dep_flecs   = require "dep_flecs"
local dep_daslang = require "dep_daslang"

local dependencies_ordered = {
    {name = "FLECS",   buildFn = dep_flecs.build},
    {name = "DASLANG", buildFn = dep_daslang.build},
}

local dependencies = {
    FLECS   = dep_flecs.build,
    DASLANG = dep_daslang.build,
}

for depName, dep in pairs(dependencies) do
    newoption {
        trigger     = depName,
        description = "Rebuild " .. depName
    }
end

function vendorDeps.getDependencyNames()
    local names = {}
    for name, _ in pairs(dependencies) do
        table.insert(names, name)
    end
    table.sort(names)
    return names
end

function vendorDeps.processDependencies()
    if not _OPTIONS["deps"] then
        return
    end

    print("Building dependencies...")
    local currentDir = os.getcwd()
    os.chdir(VENDOR_DIR)
    utils.printCurrentDir()

    local flagged = {}
    for name, buildFn in pairs(dependencies) do
        if _OPTIONS[name] then
            table.insert(flagged, name)
        end
    end

    if #flagged > 0 then
        print("Processing only flagged dependencies:")
        for _, name in ipairs(flagged) do
            print(" -> " .. name)
            dependencies[name]()
        end
    else
        print("No specific flags; processing all dependencies:")
        for _, dep in ipairs(dependencies_ordered) do
            print(" -> " .. dep.name)
            dep.buildFn()
        end
    end

    os.chdir(currentDir)
end

return vendorDeps
