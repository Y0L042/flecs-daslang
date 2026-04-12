-- flecs_das is included by the Duin workspace, which sets all FLECS_DAS_* variables
-- before this file is included. Paths are relative to the Duin solution root.

project "flecs_das"
kind "StaticLib"
language "C++"
cppdialect "C++17"

targetdir("%{wks.location}/Duin/vendor/flecs-daslang/flecs_das/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{wks.location}/Duin/vendor/flecs-daslang/flecs_das/bin-int/" .. outputdir .. "/%{prj.name}")

files { "src/**.h", "src/**.cpp", "src/**.c", "src/**.hpp", "src/**.inc" }

includedirs { "src" }

externalincludedirs {
    FLECS_DAS_FLECS_INCLUDE,
    FLECS_DAS_DASLANG_INCLUDE,
}

defines {
    "flecs_STATIC",
    "DAS_ENABLE_EXCEPTIONS=1",
    "DAS_SMART_PTR_DEBUG=1",
    "DAS_ENABLE_DLL=1",
    "DAS_MOD_EXPORTS",
}

-- Static libs must not link other static libs — consumers (executables) own those links.
-- libdirs are kept so premake generates a valid project, but no AdditionalDependencies.
libdirs {
    FLECS_DAS_FLECS_LIBDIR,
    FLECS_DAS_DASLANG_LIBDIR,
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
libdirs {
    FLECS_DAS_FLECS_LIBDIR_REL,
    FLECS_DAS_DASLANG_LIBDIR_REL,
}
filter {}
