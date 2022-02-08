project "msdfgen"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	files {
		"core/*.h",
		"core/*.hpp",
		"core/*.cpp",
		"ext/*.h",
		"ext/*.hpp",
		"ext/*.cpp",
		"include/*.h",
		"lib/*.cpp",
	}

	includedirs {
		"include",
		"../freetype/include",
		"skia/include",
		"skia/include/core",
		"skia/include/config"
	}

	defines {
		"MSDFGEN_USE_CPP11"
	}

	warnings "Off"

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		buildoptions {
			"/openmp"
		}
		defines {
			"MSDFGEN_USE_OPENMP"
		}
		links {
			"../freetype/lib/windows-x86_64/freetype.lib"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
