workspace "Escape"
    configurations { "Debug", "Release", "Shipping" }
    architecture "x86_64"
    flags "MultiProcessorCompile"
    startproject "Escape"

    targetdir ("Build/Bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("Build/Obj/%{cfg.buildcfg}/%{prj.name}")
    
group "Core"
    project "Core"
        kind "None"
        language "C++"
        cppdialect "C++latest"
        location "%{prj.name}"
        staticruntime "Off"

        files
        {
            "%{prj.name}/Source/**.cpp",
            "%{prj.name}/Source/**.h"
        }

        includedirs "%{prj.name}/Source"
        
        filter "system:windows"
            systemversion "latest"
            defines "ESCAPE_PLATFORM_WINDOWS"

        filter "configurations:Debug"
            defines "ESCAPE_BUILD_DEBUG"
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines "ESCAPE_BUILD_RELEASE"
            runtime "Release"
            optimize "On"

        filter "configurations:Shipping"
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
        "%{prj.name}/Libraries/glm/glm/**.inl",

        "%{prj.name}/Libraries/stb_image/stb_image.h"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{wks.location}/Core/Source",
        "%{wks.location}/Core/Libraries/ENet/include",
        
        "%{prj.name}/Libraries/GLFW/include",
        "%{prj.name}/Libraries/Glad/include",
        "%{prj.name}/Libraries/Box2D/include",
        "%{prj.name}/Libraries/glm",
        "%{prj.name}/Libraries/stb_image"
    }

    links
    {
        "Core",
        "ENet",

        "GLFW",
        "Glad",
        "Box2D",

        "Ws2_32.lib",
        "Mswsock.lib",
        "AdvApi32.lib",
        "secur32.lib",
        "winmm.lib",
        "dmoguids.lib",
        "wmcodecdspuuid.lib",
        "msdmo.lib",
        "Strmiids.lib"
    }

    defines
    {
        "GLM_FORCE_DEPTH_ZERO_TO_ONE"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "ESCAPE_PLATFORM_WINDOWS",
            "NOMINMAX"
        }
    
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
        "%{wks.location}/Core/Libraries/ENet/include"
    }
    
    links
    {
        "Core",
        "ENet",
    
        "Ws2_32.lib",
        "secur32.lib",
        "winmm.lib",
        "dmoguids.lib",
        "wmcodecdspuuid.lib",
        "msdmo.lib",
        "Strmiids.lib"
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
    -- Core
    include "Core/Libraries/ENet"

    -- Client    
    include "Client/Libraries/GLFW"
    include "Client/Libraries/Glad"
    include "Client/Libraries/Box2D"
group ""