cmake_minimum_required(VERSION 3.14)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(Article)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WIN32)
    add_compile_definitions(LI_PLATFORM_WINDOWS)
elseif (UNIX)
    add_compile_definitions(LI_PLATFORM_LINUX)
endif()

add_compile_definitions(
    $<$<CONFIG:Debug>:LI_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:LI_DEBUG>
    $<$<CONFIG:Release>:LI_RELEASE>
    $<$<CONFIG:MinSizeRel>:LI_RELEASE>
)

add_subdirectory(Lithium)
add_subdirectory(Article)
add_subdirectory(AssetBase)
