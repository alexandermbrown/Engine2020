#include "pch.h"
#include "ArticleApp.h"

#include "Article/Terrain/NoiseStore.h"
#include "Article/Scenes/SplashScreenScene.h"
#include "Article/ECS/RegisterSystems.h"

#ifndef LI_DIST
#include "Article/Console/CVarCommands.h"
#include "Article/Layers/ConsoleLayer.h"
#endif

#include "Lithium/Localization/Localization.h"
#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Resources/ResourceManager.h"

#ifdef HZ_PLATFORM_WINDOWS
#include <shlobj.h>
#endif

ArticleApp::ArticleApp()
	: Li::Application({ "Article", 768, 384, false, true, true})
{
#ifdef HZ_DEBUG
	Li::ResourceManager::Load("data/preload.lab-debug");
#else
	Li::ResourceManager::Load("data/preload.lab");
#endif
	Li::Localization::Init();
	Li::Renderer::Init();

	RegisterSystems(m_SystemRegistry);
	LoadConfig();

	m_ScriptContext.InitApp("data/scripts/Init.lua");
	
#ifdef HZ_CONSOLE_ENABLED
	PushOverlay(Li::MakeUnique<ConsoleLayer>());

	m_Console.AddCommand(CreateCVarSetCommand());
	m_Console.AddCommand(CreateCVarGetCommand());
#endif

	uint32_t game_to_app = SDL_RegisterEvents(1);
	uint32_t app_to_game = SDL_RegisterEvents(1);

	LI_ASSERT(game_to_app != ((uint32_t)-1), "Failed to create user event.");
	LI_ASSERT(app_to_game != ((uint32_t)-1), "Failed to create user event.");

	// TODO: add unsigned int.
	m_ConfigStore.Set<int>("event_game_to_app", game_to_app);
	m_ConfigStore.Set<int>("event_app_to_game", app_to_game);
	m_ConfigStore.Set<int>("app_state", static_cast<int>(AppState::None));

	Transition(Li::MakeUnique<SplashScreenScene>(), true);
}

ArticleApp::~ArticleApp()
{
	// Save window size.
	AppState app_state = static_cast<AppState>(m_ConfigStore.Get<int>("app_state"));
	if (app_state != AppState::None && app_state != AppState::SplashScreen)
	{
		Li::Window& window = GetWindow();
		m_ConfigStore.Set<int>("window_width", window.GetWidth());
		m_ConfigStore.Set<int>("window_height", window.GetHeight());
	}

	ClearScene();
	ClearLayers();
	NoiseStore::Shutdown();
	SaveConfig();
}

void ArticleApp::LoadConfig()
{
	m_ConfigLoader.RegisterDefaults("data/config/config_default.yaml");

#ifdef HZ_PLATFORM_WINDOWS
	// Get the location of %APPDATA% in Windows.
	PWSTR path_str;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_str);

	std::filesystem::path path(path_str);
	path /= "Article/config/config.yaml";

	m_ConfigStore = m_ConfigLoader.LoadStore(path);

	CoTaskMemFree(static_cast<LPVOID>(path_str));
#endif
}

void ArticleApp::SaveConfig()
{
#ifdef HZ_PLATFORM_WINDOWS
	// Get the location of %APPDATA% in Windows.
	PWSTR path_str;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_str);

	std::filesystem::path path(path_str);
	path /= "Article/config/config.yaml";

	m_ConfigLoader.SaveStore(m_ConfigStore, path);

	CoTaskMemFree(static_cast<LPVOID>(path_str));
#endif
}

Li::Unique<Li::Application> Li::CreateApplication()
{
	try {
		return Li::MakeUnique<ArticleApp>();
	}
	catch (const std::runtime_error& error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", error.what(), NULL);
		abort();
	}
}
