#include "lipch.h"
#include "GLContext.h"

#include "Lithium/Core/Assert.h"
#include "ConvertGL.h"
#include "GLCore.h"

#include "glad/glad.h"

#include <stdexcept>
#include <sstream>

namespace Li
{
	void GLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* user_param)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         Log::CoreFatal(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       Log::CoreError(message); return;
		case GL_DEBUG_SEVERITY_LOW:          Log::CoreWarn(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: Log::CoreTrace(message); return;
		}

		LI_CORE_ASSERT(false, "Unknown severity level!");
	}

	GLContext::GLContext(SDL_Window* window_handle, int width, int height)
		: m_WindowHandle(window_handle), m_Width(width), m_Height(height), m_DrawMode(DrawMode::Triangles)
	{
		m_Context = SDL_GL_CreateContext(window_handle);

		SDL_GL_MakeCurrent(m_WindowHandle, m_Context);
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
			throw std::runtime_error("Failed to load OpenGL functions.");

		Log::CoreInfo("OpenGL Info:");
		GLCall(Log::CoreInfo("    Vendor: {0}",         (const char*)glGetString(GL_VENDOR)));
		GLCall(Log::CoreInfo("    Renderer: {0}",       (const char*)glGetString(GL_RENDERER)));
		GLCall(Log::CoreInfo("    Version: OpenGL {0}", (const char*)glGetString(GL_VERSION)));
		
		// Verify OpenGL version.
		int version_major;
		int version_minor;
		GLCall(glGetIntegerv(GL_MAJOR_VERSION, &version_major));
		GLCall(glGetIntegerv(GL_MINOR_VERSION, &version_minor));

		struct GPUInfo
		{
			int max_compute_ssbo_count = -1;
			int max_ssbo_bindings = -1;
		};
		GPUInfo info;

		GLCall(glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &info.max_compute_ssbo_count));
		GLCall(glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &info.max_ssbo_bindings));
		Log::CoreDebug("Max Compute SSBO Count: {}", info.max_compute_ssbo_count);
		Log::CoreDebug("Max SSBO Bindings: {}", info.max_ssbo_bindings);
		

#ifdef LI_DEBUG
		constexpr int RequiredMajor = 4;
		constexpr int RequiredMinor = 3;
#else
		constexpr int RequiredMajor = 4;
		constexpr int RequiredMinor = 2;
#endif
		bool good_version = version_major > RequiredMajor || (version_major == RequiredMajor && version_minor >= RequiredMinor);
		if (!good_version)
		{
			std::stringstream stream;
			stream << "OpenGL version " << RequiredMajor << "." << RequiredMinor << " is required.";
			throw std::runtime_error(stream.str());
		}
		
#ifdef LI_DEBUG
		GLCall(glEnable(GL_DEBUG_OUTPUT));
		GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		GLCall(glDebugMessageCallback(GLMessageCallback, nullptr));

		GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE));
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		m_DepthTest = true;
	}

	GLContext::~GLContext()
	{
		SDL_GL_MakeCurrent(m_WindowHandle, nullptr);
		SDL_GL_DeleteContext(m_Context);
	}

	void GLContext::BindDefaultRenderTarget()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_Width, m_Height);
	}

	void GLContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLContext::CommandBarrier()
	{
		glMemoryBarrier(GL_COMMAND_BARRIER_BIT);
	}

	void GLContext::DrawArrays(uint32_t vertexCount)
	{
		glDrawArrays(ConvertGL::DrawMode(m_DrawMode), 0, (GLsizei)vertexCount);
	}

	void GLContext::DrawIndexed(uint32_t indexCount)
	{
		glDrawElements(ConvertGL::DrawMode(m_DrawMode), indexCount, GL_UNSIGNED_INT, nullptr);
	}

	void GLContext::DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount)
	{
		glDrawElementsInstanced(ConvertGL::DrawMode(m_DrawMode),
			indexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
	}

	void GLContext::DispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z)
	{
		glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
	}

	void GLContext::ShaderStorageBarrier()
	{
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void GLContext::ResizeView(int width, int height)
	{
		m_Width = width;
		m_Height = height;
		glViewport(0, 0, m_Width, m_Height);
	}

	void GLContext::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GLContext::SetDepthTest(bool enabled)
	{
		if (m_DepthTest == enabled)
			return;

		m_DepthTest = enabled;

		if (enabled) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	void GLContext::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}

	void GLContext::UniformBufferBarrier()
	{
		glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
	}

	void GLContext::UnbindVertexArray()
	{
		glBindVertexArray(0);
	}
}
