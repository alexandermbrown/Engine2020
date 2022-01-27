workspace "Article"
	architecture "x86_64"
	startproject "Article"
	linkgroups "On"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "Lithium/vendor/spdlog/include"
IncludeDir["SDL2"] = "Lithium/vendor/SDL2/include"
IncludeDir["glad"] = "Lithium/vendor/glad/include"
IncludeDir["glm"] = "Lithium/vendor/glm"
IncludeDir["imgui"] = "Lithium/vendor/imgui"
IncludeDir["stb_image"] = "Lithium/vendor/stb_image"
IncludeDir["zlib"] = "Lithium/vendor/zlib"
IncludeDir["openal"] = "Lithium/vendor/openal-soft/include"
IncludeDir["libogg"] = "Lithium/vendor/libogg/include"
IncludeDir["libvorbis"] = "Lithium/vendor/libvorbis/include"
IncludeDir["simpleini"] = "Lithium/vendor/simpleini/include"
IncludeDir["readerwriterqueue"] = "Lithium/vendor/readerwriterqueue/include"
IncludeDir["harfbuzz"] = "Lithium/vendor/harfbuzz/include"
IncludeDir["utfcpp"] = "Lithium/vendor/utfcpp/include"
IncludeDir["libav"] = "Lithium/vendor/libav/include"

IncludeDir["assimp"] = "AssetBase/vendor/assimp/include"
IncludeDir["flatbuffers"] = "AssetBase/vendor/flatbuffers/include"
IncludeDir["freetype"] = "AssetBase/vendor/freetype/include"
IncludeDir["msdfgen"] = "AssetBase/vendor/msdfgen"
IncludeDir["msdfatlasgen"] = "AssetBase/vendor/msdf-atlas-gen"
IncludeDir["yamlcpp"] = "AssetBase/vendor/yaml-cpp/include"
IncludeDir["ShaderConductor"] = "AssetBase/vendor/ShaderConductor/include"
IncludeDir["lab_serial"] = "FlatBuffers/include"

IncludeDir["entt"] = "Article/vendor/entt/include"
IncludeDir["box2d"] = "Article/vendor/box2d/include"
IncludeDir["steam"] = "Article/vendor/steam/include"
IncludeDir["layout"] = "Article/vendor/layout/include"
IncludeDir["lua"] = "Article/vendor/lua/include"
IncludeDir["sol"] = "Article/vendor/sol"
IncludeDir["nativefiledialog"] = "Article/vendor/nativefiledialog/src/include"

group "vendor"
include "Lithium/vendor/glad"
include "Lithium/vendor/imgui"
include "Lithium/vendor/zlib"
include "AssetBase/vendor/msdf-atlas-gen"
include "AssetBase/vendor/yaml-cpp"
include "Article/vendor/box2d"
include "Article/vendor/nativefiledialog"
group  ""

------------------------------ Lithium ----------------------------------

LithiumLibs = {
	"glad",
	"zlib",
	"ogg",
	"vorbis",
	"openal",
	"SDL2",
	"harfbuzz",
	"vorbis",
	"vorbisenc",
	"vorbisfile"
}

LithiumLibs_NOTDIST = { "imgui" }

LithiumLibDirs = {
	"Lithium/vendor/libogg/lib/%{cfg.system}-%{cfg.architecture}",
	"Lithium/vendor/libvorbis/lib/%{cfg.system}-%{cfg.architecture}",
	"Lithium/vendor/openal-soft/lib/%{cfg.system}-%{cfg.architecture}",
	"Lithium/vendor/harfbuzz/lib/%{cfg.system}-%{cfg.architecture}",
	"Lithium/vendor/SDL2/lib/%{cfg.system}-%{cfg.architecture}"
}

LithiumLibDirs_NOTDIST = { "%{IncludeDir.imgui}" }

project "Lithium"
	location "Lithium"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	pchheader "lipch.h"
	pchsource "Lithium/src/lipch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/glm/glm/**.h",
	}

	includedirs {
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.zlib}",
		"%{IncludeDir.openal}",
		"%{IncludeDir.libogg}",
		"%{IncludeDir.libvorbis}",
		"%{IncludeDir.readerwriterqueue}",
		"%{IncludeDir.flatbuffers}",
		"%{IncludeDir.lab_serial}",
		"%{IncludeDir.harfbuzz}",
		"%{IncludeDir.utfcpp}",
		"%{IncludeDir.libav}"
	}

	filter "system:windows"
		systemversion "latest"

		libdirs {
			"Lithium/vendor/SDL2/lib/win64",
		}

		defines {
			"LI_PLATFORM_WINDOWS"
		}

		links {
			"d3d11.lib",
			"dxgi.lib",
			"d3dcompiler.lib",
			-- libav
			"Ws2_32.lib",
			"Strmiids.lib",
			"Secur32.lib",
			"mfplat.lib",
			"mfuuid.lib",
			"bcrypt.lib",
			"avcodec",
			"avdevice",
			"avformat",
			"avutil",
			"opus",
			"swresample",
			"swscale"
		}

	filter { "system:windows", "configurations:Dist"}
		libdirs "Lithium/vendor/libav/build/Release-windows-x86_64"
	filter { "system:windows", "configurations:not Dist"}
		libdirs ("Lithium/vendor/libav/build/" .. outputdir)
	
	filter "system:linux"
		-- buildoptions { "-fpermissive" }
		excludes {
			"Lithium/src/Lithium/Platform/D3D11/*",
			"Lithium/src/Lithium/VideoPlayer/*"
		}
		defines {
			"LI_PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines "LI_DEBUG"
		runtime "Debug"
		symbols "on"
		
		-- links(LithiumLibs_NOTDIST)
		includedirs(LithiumLibDirs_NOTDIST)
		-- links "vpxmdd"

	filter "configurations:Release"
		defines "LI_RELEASE"
		runtime "Release"
		optimize "on"

		-- links(LithiumLibs_NOTDIST)
		includedirs(LithiumLibDirs_NOTDIST)
		-- links "vpxmd"

	filter "configurations:Dist"
		defines "LI_DIST"
		runtime "Release"
		optimize "on"

		-- links "vpxmd"

------------------------------ Article ------------------------------

project "Article"
	location "Article"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	dpiawareness "HighPerMonitor"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Article/src/pch.cpp"
	
	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Lithium/src",
		"Article/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.readerwriterqueue}",
		"%{IncludeDir.layout}",
		"%{IncludeDir.simpleini}",
		"%{IncludeDir.steam}",
		"%{IncludeDir.flatbuffers}",
		"%{IncludeDir.lab_serial}",
		"%{IncludeDir.harfbuzz}",
		"%{IncludeDir.lua}",
		"%{IncludeDir.sol}",
		"%{IncludeDir.nativefiledialog}",
		"%{IncludeDir.yamlcpp}"
	}

	links(LithiumLibs)
	links {
		"Lithium",
		"box2d",
		"lua",
		"nativefiledialog",
		"yaml-cpp"
	}

	libdirs(LithiumLibDirs)
	libdirs {
		-- "Article/vendor/steam/lib/%{cfg.system}-%{cfg.architecture}",
		"Article/vendor/lua/lib/%{cfg.system}-%{cfg.architecture}"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"LI_PLATFORM_WINDOWS"
		}

		links {
			"lua51",
			"steam_api64"
		}
	
	filter "system:linux"
		defines {
			"LI_PLATFORM_LINUX"
		}
		linkoptions "-Wl,-rpath,'$$ORIGIN'"
		links {
			"dl",
			"pthread"
		}

	filter "configurations:Debug"
		defines {
			"LI_DEBUG",
			"HZ_DEBUG",
			"HZ_PHYSICS_DEBUG_DRAW",
			"HZ_CONSOLE_ENABLED"
		}
		runtime "Debug"
		symbols "on"
		
		links(LithiumLibs_NOTDIST)
		includedirs(LithiumLibDirs_NOTDIST)

	filter "configurations:Release"
		defines {
			"LI_RELEASE",
			"HZ_RELEASE",
			"HZ_CONSOLE_ENABLED"
		}
		runtime "Release"
		optimize "on"
		
		links(LithiumLibs_NOTDIST)
		includedirs(LithiumLibDirs_NOTDIST)

	filter "configurations:Dist"
		defines {
			"LI_DIST",
			"HZ_DIST"
		}
		runtime "Release"
		optimize "on"

------------------------------ AssetBase ------------------------------

project "AssetBase"
	location "AssetBase"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("build/" .. outputdir .. "/%{prj.name}")
	objdir ("build-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "AssetBase/src/pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"AssetBase/src",
		"Lithium/src",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.flatbuffers}",
		"%{IncludeDir.freetype}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.lab_serial}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.msdfatlasgen}",
		"%{IncludeDir.ShaderConductor}",
		"%{IncludeDir.yamlcpp}",
		"%{IncludeDir.zlib}"
	}

	links {
		"msdfgen",
		"msdf-atlas-gen",
		"zlib",
		"yaml-cpp",
		"ShaderConductor",
		"freetype",
		"assimp"
	}
	
	libdirs "AssetBase/vendor/ShaderConductor/lib/Debug-%{cfg.system}-%{cfg.architecture}"
	libdirs "AssetBase/vendor/freetype/lib/%{cfg.system}-%{cfg.architecture}"
	libdirs "AssetBase/vendor/assimp/lib/%{cfg.system}-%{cfg.architecture}"
	filter "system:windows"
		systemversion "latest"

		links {
			"d3d11.lib",
			"dxgi.lib",
			"d3dcompiler.lib"
		}

		defines {
			"LI_PLATFORM_WINDOWS"
		}
	
	filter "system:linux"
		linkoptions "-Wl,-rpath,'$$ORIGIN'"
		defines {
			"LI_PLATFORM_LINUX"
		}

		links {
			"pthread"
		}
	
	--filter { "system:windows", "configurations:Debug" }
		--libdirs "AssetBase/vendor/ShaderConductor/lib/win64-debug"
		-- links {
		-- 	"spirv-cross-cored",
		-- 	"spirv-cross-glsld",
		-- 	"spirv-cross-hlsld",
		-- 	"spirv-cross-msld"
		-- }
	
	--filter { "system:windows", "configurations:Release or Dist" }
		-- links {
		-- 	"spirv-cross-core",
		-- 	"spirv-cross-glsl",
		-- 	"spirv-cross-hlsl",
		-- 	"spirv-cross-msl"
		-- }

	filter "configurations:Debug"
		defines "LI_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "LI_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "LI_DIST"
		runtime "Release"
		optimize "on"
