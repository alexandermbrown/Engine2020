project "lua"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.c"
	}

	warnings "Off"

	filter "system:windows"
		systemversion "latest"

		defines {
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		defines { "NDEBUG" }

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
		defines { "NDEBUG" }
