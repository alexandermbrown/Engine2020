#pragma once

#include "Lithium/Core/Scene.h"
#include "Lithium/Core/Memory.h"
#include <string>

Li::Unique<Li::Scene> GetNativeScene(const std::string& name);
