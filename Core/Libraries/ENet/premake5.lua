project "ENet"
    kind "StaticLib"
    language "C"
    staticruntime "Off"

    files
    {
        "src/**.c",
        "include/**.h"
    }

    includedirs
    {
        "include",
        "src"
    }

    filter "system:windows"
        systemversion "latest"
        defines "_WINSOCK_DEPRECATED_NO_WARNINGS"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        defines "DEBUG"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        defines "NDEBUG"

    filter "configurations:Shipping"
        runtime "Release"
        optimize "On"
        symbols "Off"
        defines "NDEBUG"