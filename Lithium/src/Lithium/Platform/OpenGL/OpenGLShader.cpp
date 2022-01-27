#include "lipch.h"
#include "Lithium/Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"

#include "OpenGLCore.h"

namespace Li
{
	OpenGLShader::OpenGLShader(const std::string& name, const GLSLInput& input)
		: m_Name(name), m_RendererID(0)
	{
		std::vector<ShaderStage> sources;
		sources.reserve(6);
		if (input.VertexSrc) {
			ShaderStage stage{"GL_VERTEX_SHADER", GL_VERTEX_SHADER, input.VertexSrc, 0};

			sources.push_back(stage);
		}
		if (input.TessControlSrc)
			sources.push_back(ShaderStage{"GL_TESS_CONTROL_SHADER", GL_TESS_CONTROL_SHADER, input.TessControlSrc, 0});
		if (input.TessEvalSrc)
			sources.push_back(ShaderStage{"GL_TESS_EVALUATION_SHADER", GL_TESS_EVALUATION_SHADER, input.TessEvalSrc, 0});
		if (input.GeometrySrc)
			sources.push_back(ShaderStage{"GL_GEOMETRY_SHADER", GL_GEOMETRY_SHADER, input.GeometrySrc, 0});
		if (input.FragmentSrc)
			sources.push_back(ShaderStage{"GL_FRAGMENT_SHADER", GL_FRAGMENT_SHADER, input.FragmentSrc, 0});
		if (input.ComputeSrc)
			sources.push_back(ShaderStage{"GL_COMPUTE_SHADER", GL_COMPUTE_SHADER, input.ComputeSrc, 0});

		Compile(std::move(sources));
	}

	OpenGLShader::~OpenGLShader()
	{
		GLCall( glDeleteProgram(m_RendererID) );
	}

	void OpenGLShader::Compile(std::vector<ShaderStage>&& stages)
	{
		GLuint program = glCreateProgram();


		for (ShaderStage& stage : stages)
		{
			stage.GlId = glCreateShader(stage.GlType);
			glShaderSource(stage.GlId, 1, &stage.Source, 0);
			glCompileShader(stage.GlId);

			GLint is_compiled = 0;
			glGetShaderiv(stage.GlId, GL_COMPILE_STATUS, &is_compiled);
			if (is_compiled == GL_FALSE)
			{
				GLint max_length = 0;
				glGetShaderiv(stage.GlId, GL_INFO_LOG_LENGTH, &max_length);

				std::vector<GLchar> info_log(max_length);
				glGetShaderInfoLog(stage.GlId, max_length, &max_length, &info_log[0]);

				glDeleteShader(stage.GlId);

				std::string err_msg = "Failed to compile stage " + stage.Type + " of \"" + m_Name + "\"" + info_log.data();
				Log::CoreError("{}", err_msg);
				throw std::runtime_error(err_msg);
			}

			glAttachShader(program, stage.GlId);
		}
		
		glLinkProgram(program);

		GLint is_linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
		if (is_linked == GL_FALSE)
		{
			GLint max_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

			glDeleteProgram(program);
			for (ShaderStage stage : stages)
			{
				glDeleteShader(stage.GlId);
				stage.GlId = 0;
			}

			std::string err_msg = "Failed to link shader \"" + m_Name + "\"\n" + info_log.data();
			Log::CoreError("{}", err_msg);
			throw std::runtime_error(err_msg);
		}

		for (ShaderStage stage : stages)
		{
			glDetachShader(program, stage.GlId);
			glDeleteShader(stage.GlId);
			stage.GlId = 0;
		}
		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
