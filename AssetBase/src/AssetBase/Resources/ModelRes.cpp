#include "pch.h"
#include "ModelRes.h"

#include "AssetBase/Util/OptionalField.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace fb = flatbuffers;

flatbuffers::Offset<Assets::Model> SerializeModel(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node mesh)
{
	Assimp::Importer importer;

	std::filesystem::path mesh_path(base_path.parent_path() / GetOptionalString(mesh, "path"));

	// TODO: check read flags.
	const aiScene* scene = importer.ReadFile(mesh_path.string(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		//aiProcess_RemoveComponent |
		aiProcess_SplitLargeMeshes |
		aiProcess_ValidateDataStructure |
		aiProcess_SortByPType |
		aiProcess_FindInvalidData |
		aiProcess_GenUVCoords |
		aiProcess_OptimizeMeshes 
	);

	if (scene == nullptr)
		throw std::runtime_error("Failed to read mesh file");

	std::vector<Assets::Submesh> submeshes(scene->mNumMeshes);
	std::vector<float> vertices;
	std::vector<uint32_t> indices;

	uint32_t vertex_count = 0;
	uint32_t index_count = 0;

	struct Vertex
	{
		// TODO: Add other components.
		float x, y, z;
	};

	for (int m = 0; m < scene->mNumMeshes; m++)
	{
		aiMesh* mesh = scene->mMeshes[m];
		
		uint32_t submesh_index_count = mesh->mNumFaces * 3;
		submeshes.push_back({
			vertex_count,
			index_count,
			mesh->mNumVertices,
			submesh_index_count
		});

		vertex_count += mesh->mNumVertices;
		index_count += submesh_index_count;

		vertices.reserve(vertex_count * sizeof(Vertex) / sizeof(float));

		for (size_t v = 0; v < mesh->mNumVertices; v++)
		{
			const aiVector3D& vert = mesh->mVertices[v];
			vertices.push_back(vert.x);
			vertices.push_back(vert.y);
			vertices.push_back(vert.z);
		}

		for (size_t f = 0; f < mesh->mNumFaces; f++)
		{
			const aiFace& face = mesh->mFaces[f];

			if (face.mNumIndices != 3)
				throw std::runtime_error("Error: Non-triangular face found!");

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
	}

	return Assets::CreateModelDirect(builder, name.c_str(), &vertices, &indices, &submeshes);
}
