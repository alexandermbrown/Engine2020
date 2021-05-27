#pragma once

#include "Article/Core/AppState.h"
#include "Article/Config/ConfigStore.h"
#include "Article/Config/ConfigLoader.h"
#include "Article/Scripting/ScriptContext.h"
#include "Article/Scripting/TerrainData.h"
#include "Article/ECS/SystemRegistry.h"

#ifndef LI_DIST
#include "Article/Console/Console.h"
#endif

#include "Lithium/Core/Application.h"

class ArticleApp : public Li::Application
{
public:
	ArticleApp(Li::RendererAPI renderer_api);
	virtual ~ArticleApp();

	inline ConfigStore& GetConfig() { return m_ConfigStore; }
	inline ScriptContext& GetScriptContext() { return m_ScriptContext; }
	inline TerrainData& GetTerrainData() { return m_TerrainData; }
	inline const SystemRegistry& GetSystemRegistry() const { return m_SystemRegistry; }

#ifndef LI_DIST
	inline Console& GetConsole() { return m_Console; }
#endif

private:
	void LoadConfig();
	void SaveConfig();

	ConfigLoader m_ConfigLoader;
	ConfigStore m_ConfigStore;

	ScriptContext m_ScriptContext;
	TerrainData m_TerrainData;
	SystemRegistry m_SystemRegistry;

#ifndef LI_DIST
	Console m_Console;
#endif
};
