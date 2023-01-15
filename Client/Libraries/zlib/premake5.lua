project "zlib"
	kind "StaticLib"
	language "C"
	staticruntime "Off"

	files
	{
		"include/**.h",
		"src/**.c"
	}

    includedirs "include"

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE"
    }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		optimize "On"
		symbols "Off"
