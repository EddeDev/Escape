project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "Off"

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs "include"

    filter "system:windows"
        systemversion "latest"