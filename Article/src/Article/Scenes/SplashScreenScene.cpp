#include "pch.h"
#include "SplashScreenScene.h"

#include "Article/Layers/SplashScreenLayer.h"
#include "Article/Layers/DiagnosticsLayer.h"

#include "ArticleApp.h"

#include "Article/Core/AppState.h"
#include "Article/Scripting/TerrainData.h"

#include "Lithium/Resources/ResourceManager.h"

void SplashScreenScene::OnShow()
{
	ArticleApp& app = Li::Application::Get<ArticleApp>();

	app.GetConfig().Set<int>("app_state", static_cast<int>(AppState::SplashScreen));

	app.PushLayer(Li::MakeUnique<SplashScreenLayer>());
#ifndef HZ_DIST
	app.PushOverlay(Li::MakeUnique<DiagnosticsLayer>());
#endif

#ifdef HZ_DEBUG
	Li::ResourceManager::BeginStaggeredLoad("data/resources-debug.lpack");
#else
	Li::ResourceManager::BeginStaggeredLoad("data/resources.lpack");
#endif
}

void SplashScreenScene::OnTransition()
{
	ArticleApp& app = Li::Application::Get<ArticleApp>();
	app.PopLayer("SplashScreen");

	int width = app.GetConfig().Get<int>("window_width");
	int height = app.GetConfig().Get<int>("window_height");

	Li::Window& window = app.GetWindow();
	window.SetBordered(true);
	window.SetSize(width, height);
	window.SetPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	window.SetResizable(true);
	window.SetIcon("data/images/icon.png");
}
