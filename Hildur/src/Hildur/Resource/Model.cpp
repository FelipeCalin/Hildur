#include "hrpcheaders.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Hildur {


	Model::Model(const std::string& path)
	{
		loadModel(path);
	}

	Model::~Model() 
	{
	}

	Ref<Model> Model::Create(const std::string& path) 
	{
		return CreateRef<Model>(path);
	}

	void Model::loadModel(const std::string& path) 
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			HR_CORE_ASSERT(false, "Could not load model: {0}", importer.GetErrorString());
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene) 
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Ref<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene) 
	{
		Ref<std::vector<Vertex>> vertices;
		Ref<std::vector<uint32_t>> indices;
		std::vector<Ref<Texture2D>> textures;

		vertices.reset(new std::vector<Vertex>);
		indices.reset(new std::vector<uint32_t>);

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			// process vertex positions, normals and texture coordinates
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = 1 - mesh->mTextureCoords[0][i].y; //Flip the y coordinates TODO: research
				vertex.texCoord = vec;
			}
			else
			{
				vertex.texCoord = glm::vec2(0.0f, 0.0f);
			}

			vertices->push_back(vertex);
		}

		// process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices->push_back(face.mIndices[j]);
		}
		
		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Ref<Texture2D>> diffuseMaps = loadMaterialTextures(material,
				aiTextureType_DIFFUSE, HR_DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Ref<Texture2D>> specularMaps = loadMaterialTextures(material,
				aiTextureType_SPECULAR, HR_SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		

		return CreateRef<Mesh>(vertices, indices, textures);
	}

	std::vector<Ref<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType) 
	{
		std::vector<Ref<Texture2D>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j]->path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{   
				// if texture hasn't been loaded already, load it
				Ref<Texture2D> texture;
				texture->Create(str.C_Str());
				texture->type = textureType;
				texture->path = str.C_Str();
				textures.push_back(texture);
			}
		}

		return textures;
	}


}