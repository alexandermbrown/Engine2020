#include "pch.h"
#include "GameScene.h"

#include "ArticleApp.h"
#include "Article/Core/AppState.h"

#include "Article/Layers/GameLayer.h"

void GameScene::OnShow()
{
	ArticleApp& app = Li::Application::Get<ArticleApp>();
	app.GetConfig().Set<int>("app_state", static_cast<int>(AppState::InGame));
	app.PushLayer(Li::MakeUnique<GameLayer>());
#ifndef LI_DIST
	app.GetImGuiRenderer()->SetBlockEvents(true);
#endif
}

void GameScene::OnTransition()
{
	ArticleApp& app = Li::Application::Get<ArticleApp>();
	app.PopLayer("GameLayer");
}
