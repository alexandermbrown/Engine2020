cmake_minimum_required(VERSION 3.12)

project(Article)

file(GLOB_RECURSE ARTICLE_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(Article ${ARTICLE_SOURCES})

target_sources(Article PRIVATE ${CMAKE_SOURCE_DIR}/dpi-aware.manifest)

target_include_directories(Article PRIVATE
	${CMAKE_SOURCE_DIR}/Lithium/src
	${CMAKE_SOURCE_DIR}/Article/src
)

target_link_libraries(Article PRIVATE Lithium)

# Dependencies
target_include_directories(Article PUBLIC ${CMAKE_SOURCE_DIR}/Article/vendor/layout/include)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(Article PRIVATE box2d::box2d)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(Article PRIVATE EnTT::EnTT)

find_package(unofficial-nativefiledialog CONFIG REQUIRED)
target_link_libraries(Article PRIVATE unofficial::nativefiledialog::nfd)

find_package(Lua REQUIRED)
target_include_directories(Article PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(Article PRIVATE ${LUA_LIBRARIES})

find_package(sol2 CONFIG REQUIRED)
target_link_libraries(Article PRIVATE sol2::sol2)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(Article PRIVATE yaml-cpp)
