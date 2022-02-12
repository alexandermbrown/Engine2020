#include "lipch.h"
#include "Application.h"

#include "Lithium/Core/Core.h"
#include "Lithium/Core/Assert.h"
#include "Lithium/Core/Exceptions.h"
#include "Lithium/Core/Log.h"
#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Resources/ResourceManager.h"
#include "Lithium/Localization/Localization.h"

#include "SDL2/SDL.h"
#ifndef LI_DIST
#include "imgui.h"
#endif

#include <iostream>
#include <filesystem>

namespace Li
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const WindowProps& props)
		: m_Running(false), m_EventHandled(false),
		m_CurrentScene(nullptr), m_NextScene(nullptr), m_TransitionFinished(true), m_CallOnTransition(false)
	{
		LI_CORE_ASSERT(s_Instance == nullptr, "Instance of Application already exists!");
		s_Instance = this;

		std::filesystem::path cwd = std::filesystem::current_path();
		Log::CoreInfo("Starting engine at {}", cwd.string());


		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cout << "Failed to init SDL: " << SDL_GetError() << std::endl;
			throw SDLInitError("Failed to initialize SDL!");
		}

		SDL_version compiled_version;
		SDL_VERSION(&compiled_version);
		Log::CoreInfo("SDL compiled ver {0}.{1}.{2}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

#ifdef LI_DEBUG
		SDL_version linked_version;
		SDL_GetVersion(&linked_version);
		Log::CoreInfo("SDL linked ver {0}.{1}.{2}", linked_version.major, linked_version.minor, linked_version.patch);

		LI_CORE_ASSERT(
			linked_version.major == SDL_MAJOR_VERSION &&
			linked_version.minor == SDL_MINOR_VERSION &&
			linked_version.patch == SDL_PATCHLEVEL,
			"SDL compiled and linked to two different versions!"
		);
#endif
		m_EventCallbackFn = LI_BIND_FN(Application::OnEvent);

		m_Window = MakeWindow(props);
		m_Window->GetContext()->ResizeView(props.Width, props.Height);
#ifndef LI_DIST
		m_ImGuiRenderer = ImGuiRenderer::Create();
#endif
		// m_AudioContext = MakeUnique<AudioContext>(nullptr);

		ResourceManager::Init();
	}

	Application::~Application()
	{
		m_CurrentScene.reset();
		m_NextScene.reset();
#ifndef LI_DIST
		m_ImGuiRenderer.reset();
#endif
		ResourceManager::Shutdown();
		Renderer::Shutdown();

		m_Window.reset();

		SDL_Quit();
	}

	void Application::Run()
	{
		m_Running = true;
		m_StartTime = std::chrono::steady_clock::now();
		m_LastUpdateTime = m_StartTime;

		while (m_Running)
		{
			//////////////////////////
			// Calculate Delta Time //
			//////////////////////////
			std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();
			Duration::us delta_time = Duration::Cast<Duration::us>(current_time - m_LastUpdateTime);
			Duration::us run_time = Duration::Cast<Duration::us>(current_time - m_StartTime);
			m_LastUpdateTime = current_time;

			//////////////////////
			// Propagate Events //
			//////////////////////
			SDL_Event sdl_event;
			while (SDL_PollEvent(&sdl_event))
				OnEvent(&sdl_event);

			//////////////////////
			// Transition Scene //
			//////////////////////
			if (m_CallOnTransition)
			{
				if (m_CurrentScene)
					m_CurrentScene->OnTransition();
				m_CallOnTransition = false;
			}

			if (m_NextScene && m_TransitionFinished)
			{
				m_CurrentScene = std::move(m_NextScene);
				m_CurrentScene->OnShow();
			}
			else if (!m_CurrentScene)
				Log::CoreWarn("No scene active.");

			///////////////////
			// Update Layers //
			///////////////////
			Renderer::BeginFrame(run_time, delta_time);

			if (!m_FocusedLayer.empty())
				m_Input.Disable();

			bool reached_focused = false;
			for (Unique<Layer>& layer : m_LayerStack)
			{
				if (m_FocusedLayer == layer->GetName() && !reached_focused)
				{
					m_Input.Enable();
					reached_focused = true;
				}
				layer->OnUpdate(delta_time);
			}

#ifndef LI_DIST
			//////////////////
			// Render ImGui //
			//////////////////
			if (!m_FocusedLayer.empty())
				m_Input.Disable();

			m_ImGuiRenderer->Begin();
			reached_focused = false;
			for (Unique<Layer>& layer : m_LayerStack)
			{
				if (m_FocusedLayer == layer->GetName() && !reached_focused)
				{
					m_Input.Enable();
					reached_focused = true;
				}
				layer->OnImGuiRender();
			}
			m_ImGuiRenderer->End();
#endif
			m_Window->SwapBuffers();
		}
	}

	void Application::OnEvent(SDL_Event* event)
	{
		m_Input.OnEvent(event);

		if (event->type == SDL_WINDOWEVENT)
			OnWindowEvent(event);

#ifndef LI_DIST
		m_ImGuiRenderer->OnEvent(event);
#endif
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (m_EventHandled)
				break;
			(*it)->OnEvent(event);
		}

		if (!m_EventHandled && event->window.event == SDL_WINDOWEVENT_CLOSE)
			m_Running = false;

		m_EventHandled = false;
	}

	void Application::Transition(Unique<Scene> next_scene, bool instant)
	{
		LI_CORE_ASSERT(m_NextScene == nullptr, "Transition already in progress!");

		m_CallOnTransition = true;
		m_TransitionFinished = instant;
		m_NextScene = std::move(next_scene);
	}

	void Application::OnWindowEvent(SDL_Event* event)
	{
		switch(event->window.event)
		{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			if (event->window.windowID == m_Window->GetWindowID())
			{
				int w, h;
				SDL_GetWindowSize(m_Window->GetWindow(), &w, &h);
				Log::CoreInfo("Resizing renderer ({0}, {1})", w, h);

				m_Window->OnWindowResize(w, h);
				m_Window->GetContext()->ResizeView(w, h);
				Renderer::Resize(w, h);
#ifndef LI_DIST
				m_ImGuiRenderer->Resize(w, h);
#endif
			}
			break;
		}
	}

	Unique<Window> Application::MakeWindow(const WindowProps& props)
	{
#if 1
#ifdef LI_PLATFORM_WINDOWS
		try {
			m_RendererAPI = RendererAPI::D3D11;
			return Window::Create(m_RendererAPI, props);
		}
		catch (const SDLWindowInitError& e) {
			Li::Log::CoreWarn("Failed to create D3D11 SDL window. {}", e.what());
		}
		catch (const GraphicsInitError& e) {
			Li::Log::CoreWarn("Failed to init Direct3D 11. {}", e.what());
		}
		Li::Log::CoreWarn("Trying OpenGL...");
#endif
#endif
		m_RendererAPI = RendererAPI::OpenGL;
		return Window::Create(m_RendererAPI, props);
	}
}
