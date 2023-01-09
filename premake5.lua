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
    location "%{prj.name}"
    staticruntime "Off"

    pchheader "EscapePCH.h"
    pchsource "%{prj.name}/Source/EscapePCH.cpp"

    files
    {
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Source/**.h"
    }

    includedirs
    {
        "%{prj.name}/Source",
        
        "%{wks.location}/Libraries/GLFW/include",
        "%{wks.location}/Libraries/Glad/include"
    }

    links
    {
        "GLFW",
        "Glad"
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

group "Libraries"
    include "Libraries/GLFW"
    include "Libraries/Glad"
group ""