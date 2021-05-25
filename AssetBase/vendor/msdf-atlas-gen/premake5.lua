include "../msdfgen"

project "msdf-atlas-gen"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	files {
		"msdf-atlas-gen/*.h",
		"msdf-atlas-gen/*.hpp",
		"msdf-atlas-gen/*.cpp",
	}

	includedirs {
		"../msdfgen",
		"../msdfgen/include",
		"../msdfgen/freetype/include"
	}

	defines {
		"MSDFGEN_USE_CPP11",
		"MSDFGEN_USE_SKIA"
	}

	warnings "Off"

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		defines {
			"MSDFGEN_USE_OPENMP",
			"_CRT_SECURE_NO_WARNINGS"
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
