
cmake_minimum_required(VERSION 3.15)

project(msdf-atlas-gen VERSION 1.2 LANGUAGES CXX)

find_package(Threads REQUIRED)

find_package(msdfgen CONFIG REQUIRED)

file(GLOB_RECURSE MSDF_ATLAS_GEN_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "msdf-atlas-gen/*.h" "msdf-atlas-gen/*.hpp")
file(GLOB_RECURSE MSDF_ATLAS_GEN_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "msdf-atlas-gen/*.cpp")

# msdf-atlas-gen library
add_library(msdf-atlas-gen ${MSDF_ATLAS_GEN_HEADERS} ${MSDF_ATLAS_GEN_SOURCES})
add_library(msdf-atlas-gen::msdf-atlas-gen ALIAS msdf-atlas-gen)
set_target_properties(msdf-atlas-gen PROPERTIES PUBLIC_HEADER "${MSDF_ATLAS_GEN_HEADERS}")
target_include_directories(msdf-atlas-gen
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/msdfgen/include # for lodepng.h
        ${CMAKE_CURRENT_SOURCE_DIR}/artery-font-format
)

target_compile_features(msdf-atlas-gen PUBLIC cxx_std_11)
target_link_libraries(msdf-atlas-gen PUBLIC Threads::Threads msdfgen::msdfgen msdfgen::msdfgen-ext)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT msdf-atlas-gen msdfgen::msdfgen-ext)

# target_link_libraries(msdf-atlas-gen PRIVATE msdfgen::msdfgen msdfgen::msdfgen-ext)
