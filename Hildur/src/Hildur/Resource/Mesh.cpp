#include "hrpcheaders.h"
#include "Mesh.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>



namespace Hildur {


	Mesh::Mesh(std::vector<glm::vec3> vertices)
	{
		Ref<std::vector<Vertex>> verticesList;
		Ref<std::vector<uint32_t>> indicesList;

		verticesList.reset(new std::vector<Vertex>);
		indicesList.reset(new std::vector<uint32_t>);

		m_BoundingBoxMin = glm::vec3(0.0f);
		m_BoundingBoxMax = glm::vec3(0.0f);

		for (int i = 0; i < vertices.size(); i++)
		{
			glm::vec3 posVec = vertices[i];
			verticesList->push_back({ posVec, glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) });
			indicesList->push_back(i);

			if (posVec.x < m_BoundingBoxMin.x)
				m_BoundingBoxMin.x = posVec.x;
			if (posVec.y < m_BoundingBoxMin.y)
				m_BoundingBoxMin.y = posVec.y;
			if (posVec.z < m_BoundingBoxMin.z)
				m_BoundingBoxMin.z = posVec.z;

			if (posVec.x > m_BoundingBoxMax.x)
				m_BoundingBoxMax.x = posVec.x;
			if (posVec.y > m_BoundingBoxMax.y)
				m_BoundingBoxMax.y = posVec.y;
			if (posVec.z > m_BoundingBoxMax.z)
				m_BoundingBoxMax.z = posVec.z;
		}

		InitVAO(verticesList, indicesList);
	}

	Mesh::Mesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices, std::vector<Ref<Texture2D>> textures)
	{
		m_BoundingBoxMin = glm::vec3(0.0f);
		m_BoundingBoxMax = glm::vec3(0.0f);

		for (int i = 0; i < vertices->size(); i++)
		{
			glm::vec3 posVec = vertices->at(i).position;

			if (posVec.x < m_BoundingBoxMin.x)
				m_BoundingBoxMin.x = posVec.x;
			if (posVec.y < m_BoundingBoxMin.y)
				m_BoundingBoxMin.y = posVec.y;
			if (posVec.z < m_BoundingBoxMin.z)
				m_BoundingBoxMin.z = posVec.z;

			if (posVec.x > m_BoundingBoxMax.x)
				m_BoundingBoxMax.x = posVec.x;
			if (posVec.y > m_BoundingBoxMax.y)
				m_BoundingBoxMax.y = posVec.y;
			if (posVec.z > m_BoundingBoxMax.z)
				m_BoundingBoxMax.z = posVec.z;
		}

		InitVAO(vertices, indices);

		for (int i = 0; i < textures.size(); i++)
		{
			switch (textures[i]->type)
			{
				// TODO: Fix!!!!
			}
		}
	}

	Mesh::~Mesh() 
	{
		m_VAO->~VertexArray();
		m_VBO->~VertexBuffer();
		m_IBO->~IndexBuffer();
	}

	Ref<Mesh> Mesh::Create(std::vector<glm::vec3>& vertices)
	{
		return CreateRef<Mesh>(vertices);
	}

	Ref<Mesh> Mesh::Create(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices, std::vector<Ref<Texture2D>> textures)
	{
		return CreateRef<Mesh>(vertices, indices, textures);
	}

	void Mesh::Update() 
	{
	}
	
	//Ref<VertexArray> Mesh::Render(Ref<Shader> shader) 
	//{
	//	UpdateUniforms(shader);
	//
	//	return m_VAO;
	//}

	//void Mesh::InitVertexData(Vertex* vertexArray, const uint32_t& numberOfVertices, uint32_t* indexArray, const uint32_t& numberOfIndices)
	//{
	//	for (size_t i = 0; i < numberOfVertices; i++) 
	//	{
	//		m_Vertices.push_back(vertexArray[i]);
	//	}
	//
	//	for (size_t i = 0; i < numberOfIndices; i++) 
	//	{
	//		m_Indices.push_back(indexArray[i]);
	//	}
	//}

	void Mesh::InitVAO(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) 
	{
		BufferLayout layout = 
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal" },
		};

		m_VBO = VertexBuffer::Create(vertices, vertices->size() * sizeof(Vertex));
		m_VBO->SetLayout(layout);

		m_IBO = IndexBuffer::Create(indices, indices->size());

		m_VAO = VertexArray::Create();
		m_VAO->AddVertexBuffer(m_VBO);
		m_VAO->SetIndexBuffer(m_IBO);

		m_VerticesCount = vertices->size();
		m_IndicesCount = indices->size();
		
		HR_CORE_INFO("Loaded mesh with {0} vertices and {1} indices", vertices->size(), indices->size());
	}

	//void Mesh::InitModelMatrix() 
	//{
	//	m_Position = glm::vec3(0.0f);
	//	m_Rotation = glm::vec3(0.0f);
	//	m_Scale = glm::vec3(1.0f);
	//
	//	m_ModelMatrix = glm::mat4(1.0f);
	//
	//	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	//	m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
	//}

	//void Mesh::UpdateUniforms(Ref<Shader> shader) 
	//{
	//	std::dynamic_pointer_cast<Hildur::OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", m_ModelMatrix);
	//}


}