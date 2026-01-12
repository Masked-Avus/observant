workspace "observant"
    configurations { "Debug", "Release" }
    location "build/"

    project "observant"
        kind "None"
        location ("build/%{prj.name}/" .. _ACTION)
        language "C++"
        cppdialect "C++11"
        files "src/**.hpp"
        includedirs "src/"

    project "observant.tests"
        kind "ConsoleApp"
        location ("build/%{prj.name}/" .. _ACTION)
        targetname "%{prj.name}"
        targetdir "bin/%{prj.name}/%{prj.cfg}"
        objdir "obj/"
        language "C++"
        cppdialect "C++11"
        files {
            "tests/**.hpp",
            "tests/**.cpp"
        }
        includedirs {
            "tests/",
            "src/"
        }
