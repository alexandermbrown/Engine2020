#include "lipch.h"
#include "Model.h"

#include "Renderer.h"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

#include <fstream>

namespace Li
{
	Model::Model(const std::string& path)
	{
		namespace fb = flatbuffers;

		std::ifstream model_file(path, std::ios::in | std::ios::binary);

		if (!model_file.good())
			throw std::runtime_error("Error opening model " + path);

		std::vector<uint8_t> buffer;

		model_file.seekg(0, std::ios::end);
		long filesize = (long)model_file.tellg();
		model_file.seekg(0, std::ios::beg);

		buffer.resize(filesize);

		model_file.read((char*)buffer.data(), filesize);
		model_file.close();

		const Assets::Model* model = fb::GetRoot<Assets::Model>(buffer.data());
		fb::Verifier verifier(buffer.data(), filesize);
		if (!model->Verify(verifier))
			throw std::runtime_error("Corrupt model " + path + ". Verify or reinstall game files.");

		m_Name = model->name()->str();

		const fb::Vector<float>* vertices = model->vertices();
		const fb::Vector<uint32_t>* indices = model->indices();

		m_Vertices = VertexBuffer::Create(vertices->data(), vertices->size() * sizeof(float), BufferUsage::StaticDraw);
		m_Indices = IndexBuffer::Create(indices->data(), indices->size(), BufferUsage::StaticDraw);

		m_Vertices->SetLayout(Renderer::GetModelPipeline()->GetSpec().Layouts[0]);
	}
}
