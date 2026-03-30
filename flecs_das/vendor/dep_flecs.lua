local utils = require "utils"
local dep_flecs = {}
local name = "FLECS"

local repo = "https://github.com/SanderMertens/flecs"
-- local commit = "2c373c3"
local tag = "v4.1.4"
local folder = "flecs"

function dep_flecs.build()
    print("START: " .. name)

    -- Clone Repo
    if not os.isdir(folder) then
        print("\t\tClone")
        utils.runCommand("git clone --recursive " .. repo .. " " .. folder)
        utils.runCommand("cd " .. folder .. " && git checkout " .. tag .. "")
    else
        print("\t\tFetch")
        utils.changeDir(folder)

        utils.runCommand("git stash")
        utils.runCommand("git pull")
        utils.runCommand("git checkout " .. tag .. "")

        utils.popDir()
    end
    print(name .. " downloaded.")


    local buildDir = "flecs/" .. VISUAL_STUDIO_BUILD_DIR
    if not os.isdir(buildDir) then
        utils.runCommand('cmake -S flecs -B ' .. buildDir .. ' -DBUILD_SHARED_LIBS=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_C_FLAGS_DEBUG="/MTd" -DCMAKE_C_FLAGS_RELEASE="/MT" -DCMAKE_CXX_FLAGS_DEBUG="/MTd" -DCMAKE_CXX_FLAGS_RELEASE="/MT"')
        utils.runCommand("cmake --build " .. buildDir .. " --config Debug")
        utils.runCommand("cmake --build " .. buildDir .. " --config Release")
    end

    print("END: " .. name)
end

return dep_flecs
