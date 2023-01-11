workspace "Escape"
    configurations { "Debug", "Release", "Shipping" }
    architecture "x86_64"
    flags "MultiProcessorCompile"
    startproject "Escape"

    targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("Build/Obj/%{cfg.buildcfg}/%{prj.name}")

group "Core"
    project "Core"
        language "C++"
        cppdialect "C++latest"
        location "%{prj.name}"
        staticruntime "Off"

        files
        {
            "%{prj.name}/Source/**.cpp",
            "%{prj.name}/Source/**.h"
        }

        includedirs
        {
            "%{prj.name}/Source"
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
group ""

project "Client"
    language "C++"
    cppdialect "C++latest"
    location "%{prj.name}"
    staticruntime "Off"

    pchheader "ClientPCH.h"
    pchsource "%{prj.name}/Source/ClientPCH.cpp"

    files
    {
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Source/**.h",

        "%{prj.name}/Libraries/glm/glm/**.h",
        "%{prj.name}/Libraries/glm/glm/**.hpp",
        "%{prj.name}/Libraries/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{wks.location}/Core/Source",
        
        "%{prj.name}/Libraries/GLFW/include",
        "%{prj.name}/Libraries/Glad/include",
        "%{prj.name}/Libraries/glm",
        "%{prj.name}/Libraries/Box2D/include"
    }

    links
    {
        "Core",

        "GLFW",
        "Glad",
        "Box2D",

        "Ws2_32.lib",
        "Mswsock.lib",
        "AdvApi32.lib"
    }

    defines "_WINSOCK_DEPRECATED_NO_WARNINGS"

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
        kind "WindowedApp"
        defines "ESCAPE_BUILD_SHIPPING"
        runtime "Release"
        optimize "On"
        symbols "Off"

project "Server"
    language "C++"
    cppdialect "C++latest"
    location "%{prj.name}"
    staticruntime "Off"

    pchheader "ServerPCH.h"
    pchsource "%{prj.name}/Source/ServerPCH.cpp"

    files
    {
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Source/**.h"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{wks.location}/Core/Source",
    }
    
    links
    {
        "Core",   
    
        "Ws2_32.lib"
    }

    defines "_WINSOCK_DEPRECATED_NO_WARNINGS"

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
    include "Client/Libraries/GLFW"
    include "Client/Libraries/Glad"
    include "Client/Libraries/Box2D"
group ""