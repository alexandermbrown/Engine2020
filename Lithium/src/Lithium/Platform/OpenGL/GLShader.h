#pragma once

#include "Lithium/Renderer/Shader.h"

#include "glm/glm.hpp"
#include <unordered_map>
#include <string>

namespace Li
{
	struct ShaderStage
	{
		std::string Type;
		unsigned int GlType;
		const char* Source;
		unsigned int GlId;
	};

	struct GLSLInput
	{
		const char* VertexSrc{ nullptr };
		const char* TessControlSrc{ nullptr };
		const char* TessEvalSrc{ nullptr };
		const char* GeometrySrc{ nullptr };
		const char* FragmentSrc{ nullptr };
		const char* ComputeSrc{ nullptr };
	};

	class GLShader : public Shader
	{
	public:
		GLShader(const std::string& name, const GLSLInput& input);
		virtual ~GLShader();

		virtual void Bind() const override;
		virtual const std::string& GetName() const override { return m_Name; }
		virtual void SetTexture(const std::string& name, int slot) override
		{
			UploadUniformInt(name, slot);
		}


		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		void Compile(std::vector<ShaderStage>&& shader_sources);

		uint32_t m_RendererID;
		std::string m_Name;
	};
}
