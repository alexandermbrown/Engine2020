#include "lipch.h"
#ifndef LI_DIST

#include "Lithium/Core/Core.h"

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_impl_sdl.h"
//#include "examples/imgui_impl_sdl.cpp"

#ifdef LI_INCLUDE_OPENGL
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui_impl_opengl3.h"
//#include "examples/imgui_impl_opengl3.cpp"
#endif

#ifdef LI_INCLUDE_D3D11
#include "imgui_impl_dx11.h"
#endif

#endif
