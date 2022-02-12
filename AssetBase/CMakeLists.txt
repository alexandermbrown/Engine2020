cmake_minimum_required(VERSION 3.12)

project(AssetBase)

file(GLOB_RECURSE ASSETBASE_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(AssetBase ${ASSETBASE_SOURCES})

set_target_properties(AssetBase PROPERTIES
	BUILD_RPATH_USE_ORIGIN TRUE
	INSTALL_RPATH $ORIGIN
)

target_include_directories(AssetBase PRIVATE
	${CMAKE_SOURCE_DIR}/AssetBase/src
	${CMAKE_SOURCE_DIR}/FlatBuffers/include
)

# Dependencies

if (WIN32)
    target_link_libraries(AssetBase PRIVATE d3d11 dxgi d3dcompiler)
endif()

if (WIN32)
	target_link_libraries(AssetBase PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ShaderConductor/lib/windows-x86_64/ShaderConductor.lib)
endif()
target_include_directories(AssetBase PRIVATE vendor/ShaderConductor/include)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(AssetBase PRIVATE assimp::assimp)

find_package(Flatbuffers CONFIG REQUIRED)
target_link_libraries(AssetBase PRIVATE flatbuffers::flatbuffers)

find_package(freetype CONFIG REQUIRED)
target_link_libraries(AssetBase PRIVATE freetype)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(AssetBase PRIVATE yaml-cpp)

find_package(msdfgen CONFIG REQUIRED)
target_link_libraries(AssetBase PRIVATE msdfgen::msdfgen msdfgen::msdfgen-ext)

add_subdirectory(vendor/msdf-atlas-gen)
target_link_libraries(AssetBase PRIVATE msdf-atlas-gen)
target_include_directories(AssetBase PRIVATE vendor/msdf-atlas-gen)


if (WIN32)
	set(SHARED_LIB_PATHS
		${CMAKE_CURRENT_SOURCE_DIR}/vendor/ShaderConductor/lib/windows-x86_64/ShaderConductor.dll
		${CMAKE_CURRENT_SOURCE_DIR}/vendor/ShaderConductor/lib/windows-x86_64/dxcompiler.dll
	)
	foreach (SHARED_LIB_PATH ${SHARED_LIB_PATHS})
	add_custom_command(TARGET AssetBase POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${SHARED_LIB_PATH}" $<TARGET_FILE_DIR:AssetBase>
	)
	endforeach()
elseif (UNIX)
	set(SHARED_LIB_PATHS
		${CMAKE_CURRENT_SOURCE_DIR}/vendor/ShaderConductor/lib/linux-x86_64/libShaderConductor.so
		${CMAKE_CURRENT_SOURCE_DIR}/vendor/ShaderConductor/lib/linux-x86_64/libdxcompiler.so
	)
	target_link_libraries(AssetBase PRIVATE ${SHARED_LIB_PATHS})
endif()

