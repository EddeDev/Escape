workspace "Escape"
    configurations { "Debug", "Release", "Shipping" }
    architecture "x86_64"
    flags "MultiProcessorCompile"
    startproject "Escape"

    targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("Build/Obj/%{cfg.buildcfg}/%{prj.name}")

project "Escape"
    language "C++"
    cppdialect "C++latest"
    staticruntime "Off"

    files
    {
        "Source/**.cpp",
        "Source/**.h"
    }

    includedirs
    {
        "Source"
    }

    filter "system:windows"
        systemversion "latest"
        defines "ESCAPE_PLATFORM_WINDOWS"
    
    filter "configurations:Debug"
        kind "ConsoleApp"
        defines "ESCAPE_BUILD_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        kind "ConsoleApp"
        defines "ESCAPE_BUILD_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Shipping"
        kind "ConsoleApp"
        defines "ESCAPE_BUILD_SHIPPING"
        runtime "Release"
        optimize "On"
        symbols "Off"