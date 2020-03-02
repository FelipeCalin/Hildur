#pragma once

#include "Hildur/Renderer/Mesh.h"
#include "Hildur/Renderer/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Hildur {


	class Model {

	public:

		Model(const std::string& path);
		~Model();

		static Ref<Model> Create(const std::string& path);

		std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }

	private:

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Ref<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
			TextureType textureType);

	private:

		std::vector<Ref<Mesh>> m_Meshes;
		std::string directory;

		std::vector<Ref<Texture2D>> textures_loaded;
	};


}