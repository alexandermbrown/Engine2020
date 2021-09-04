#pragma once

#include "Resources/FontLoader.h"
#include "yaml-cpp/yaml.h"

#include <vector>
#include <string>

class AssetBase
{
public:
	AssetBase();

	FontLoader& GetFontSerializer() { return m_FontSerializer; }
	const std::vector<std::string>& GetIncludePaths() const { return m_IncludePaths; }
	inline static AssetBase* Get() { return s_Instance; }

private:
	void CompileAssetPack(const std::string& yaml_path, const std::string& out_path, bool debug_mode);

	FontLoader m_FontSerializer;
	std::vector<std::string> m_IncludePaths;
	std::filesystem::path m_ModelsPath;

	static AssetBase* s_Instance;
};
