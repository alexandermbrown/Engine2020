cmake_minimum_required(VERSION 3.12)

project(Lithium)

set (EXCLUDE_DIR "src/Lithium/Platform/D3D11")
file (GLOB_RECURSE LITHIUM_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
foreach (TMP_PATH ${LITHIUM_SOURCES})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM LITHIUM_SOURCES ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

add_library(Lithium ${LITHIUM_SOURCES})

target_include_directories(Lithium PRIVATE
	${CMAKE_SOURCE_DIR}/Lithium/src
)

target_include_directories(Lithium PUBLIC ${CMAKE_SOURCE_DIR}/FlatBuffers/include)


if (WIN32)
    target_link_libraries(Lithium PRIVATE d3d11.lib dxgi.lib d3dcompiler.lib)
endif()

# Dependencies

find_package(glad CONFIG REQUIRED)
target_link_libraries(Lithium PRIVATE glad::glad)

find_package(glm CONFIG REQUIRED)
target_link_libraries(Lithium PRIVATE glm::glm)

find_package(harfbuzz CONFIG REQUIRED)
target_link_libraries(Lithium PRIVATE harfbuzz harfbuzz::harfbuzz)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Lithium PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Lithium PRIVATE imgui::imgui)

find_package(Ogg CONFIG REQUIRED)
target_link_libraries(Lithium PRIVATE Ogg::ogg)

find_package(Vorbis CONFIG REQUIRED)
target_link_libraries(Lithium PRIVATE Vorbis::vorbis Vorbis::vorbisenc Vorbis::vorbisfile)

find_path(READERWRITERQUEUE_INCLUDE_DIRS "readerwriterqueue/atomicops.h")
target_include_directories(Lithium PRIVATE ${READERWRITERQUEUE_INCLUDE_DIRS})

find_path(SIMPLEINI_INCLUDE_DIRS "ConvertUTF.c")
target_include_directories(Lithium PRIVATE ${SIMPLEINI_INCLUDE_DIRS})

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(Lithium PRIVATE ${STB_INCLUDE_DIRS})

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(Lithium PUBLIC spdlog::spdlog spdlog::spdlog_header_only)


