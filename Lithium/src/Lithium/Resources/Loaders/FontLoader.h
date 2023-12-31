#pragma once

#include "Lithium/Renderer/Text/Font.h"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace Li::Loaders
{
	Ref<Font> LoadFont(const Assets::Font* font);
}
