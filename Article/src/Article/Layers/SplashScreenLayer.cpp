#include "pch.h"
#include "SplashScreenLayer.h"

#include "Article/Scripting/ScriptScene.h"

#include "ArticleApp.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Lithium/Localization/Localization.h"
#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Resources/ResourceManager.h"

SplashScreenLayer::SplashScreenLayer()
	: Layer("SplashScreen")
{
	m_Texture = Li::ResourceManager::GetTexture2D("texture_splash");

	CalculateTransform();
}

void SplashScreenLayer::OnUpdate(Li::Duration::us dt)
{
	Li::Application::Get().GetWindow().GetContext()->Clear();

	if (!Li::ResourceManager::UpdateStaggeredLoad())
	{
		ArticleApp& app = Li::Application::Get<ArticleApp>();

		Li::ResourceManager::PrintInfo();

		// TODO: Make the renderer not dependant on the resource manager
		// Instead, change the below function to set the shaders used for instancing, fonts, etc.
		// In future, move the ResourceManager into Article and remove any references in Lithium.
		Li::Renderer::AddTextureAtlas(Li::ResourceManager::GetTextureAtlas("atlas_default"));
		Li::Renderer::AddTextureAtlas(Li::ResourceManager::GetTextureAtlas("atlas_terrain_earth"));

		app.GetTerrainData().LoadTypes();

		// TODO: Get locale from config.
		Li::Localization::SetLocale("en-us");

		app.Transition(Li::MakeUnique<ScriptScene>("MainMenuScene"), true);
	}
	Li::Renderer::UIRenderQuadImmediate(m_Texture.get(), m_Transform);
}
#ifndef LI_DIST
void SplashScreenLayer::OnImGuiRender()
{
}
#endif
void SplashScreenLayer::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
	{
		CalculateTransform();
	}
}

void SplashScreenLayer::CalculateTransform()
{
	Li::Window& window = Li::Application::Get().GetWindow();

	float windowAspect = (float)window.GetWidth() / (float)window.GetHeight();
	float textureAspect = (float)m_Texture->GetWidth() / (float)m_Texture->GetHeight();

	if (windowAspect >= textureAspect)
	{
		m_Transform = glm::translate(glm::mat4(1.0f), { 
				0.0f,
				0.5f * (window.GetHeight() - window.GetWidth() / textureAspect),
				0.0f
			}) * glm::scale(glm::mat4(1.0f), {
				window.GetWidth(),
				window.GetWidth() / textureAspect,
				1.0f
			});
	}
	else
	{
		m_Transform = glm::translate(glm::mat4(1.0f), {
				0.5f * (window.GetWidth() - window.GetHeight() * textureAspect),
				0.0f,
				0.0f
			}) * glm::scale(glm::mat4(1.0f), {
				window.GetHeight() * textureAspect,
				window.GetHeight(),
				1.0f
			});
	}
}
