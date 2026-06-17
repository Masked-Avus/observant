workspace "observant"
    configurations { "debug", "release" }
    location "build/"

    project "observant"
        kind "None"
        location ("build/%{prj.name}/" .. _ACTION)
        language "C++"
        cppdialect "C++11"
        warnings "Extra"
        fatalwarnings "All"
        files "src/**.hpp"
        includedirs "src/"

    project "observant-tests"
        kind "ConsoleApp"
        location ("build/%{prj.name}/" .. _ACTION)
        targetname "%{prj.name}"
        targetdir "bin/%{prj.name}/%{cfg.buildcfg}"
        objdir "obj/"
        language "C++"
        cppdialect "C++11"
        warnings "Extra"
        fatalwarnings "All"
        files {
            "tests/**.hpp",
            "tests/**.cpp"
        }
        includedirs {
            "tests/",
            "src/"
        }
