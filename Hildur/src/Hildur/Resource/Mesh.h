#pragma once

#include "Hildur/Renderer/Buffer.h"
#include "Hildur/Renderer/VertexArray.h"
#include "Hildur/Resource/Texture.h"

#include <glm/glm.hpp>


namespace Hildur {


	class Mesh 
	{
	public:

		struct Config
		{
			bool forceInit;

			Config(bool forceInit = false) :
				forceInit(forceInit)
			{
			}
		};

		Mesh(std::vector<glm::vec3> vertices);
		Mesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices, std::vector<Ref<Texture2D>> textures);
		~Mesh();

		static Ref<Mesh> Create(std::vector<glm::vec3>& vertices);
		static Ref<Mesh> Create(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices, std::vector<Ref<Texture2D>> textures);

		void Update();
		//Ref<VertexArray> Render(Ref<Shader> shader);

		Ref<VertexArray> GetVertexArray() const { return m_VAO; }
		Ref<Texture2D> GetDiffuseTex() const { return m_DiffuseTex; }
		Ref<Texture2D> GetSpecularTex() const { return m_SpecularTex; }

	private:

		//void InitVertexData(Vertex* vertexArray, const uint32_t& numberOfVertices, uint32_t* indexArray, const uint32_t& numberOfIndices);
		void InitVAO(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices);
		//void InitModelMatrix();

		//void UpdateUniforms(Ref<Shader> shader);//TODO: reference material instead

	private:

		uint32_t m_VerticesCount = 0;
		uint32_t m_IndicesCount = 0;

		Ref<VertexArray> m_VAO;
		Ref<VertexBuffer> m_VBO;
		Ref<IndexBuffer> m_IBO;

		Ref<Texture2D> m_DiffuseTex;
		Ref<Texture2D> m_SpecularTex;
	};


}