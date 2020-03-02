#include "hrpcheaders.h"
#include "Mesh.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>



namespace Hildur {


	Mesh::Mesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices, std::vector<Ref<Texture2D>> textures) {

		InitVAO(vertices, indices);

		for (int i = 0; i < textures.size(); i++) {

			switch (textures[i]->type) {

				case HR_DIFFUSE: m_DiffuseTex = textures[i];
				case HR_SPECULAR: m_SpecularTex = textures[i];

			}

		}

	}

	Mesh::~Mesh() {

		m_VAO->~VertexArray();
		m_VBO->~VertexBuffer();
		m_IBO->~IndexBuffer();

	}

	Ref<Mesh> Mesh::Create(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices, std::vector<Ref<Texture2D>> textures) {

		return std::make_shared<Mesh>(vertices, indices, textures);

	}

	void Mesh::Update() {



	}

	//Ref<VertexArray> Mesh::Render(Ref<Shader> shader) {

	//	UpdateUniforms(shader);

	//	return m_VAO;

	//}

	//void Mesh::InitVertexData(Vertex* vertexArray, const uint32_t& numberOfVertices, uint32_t* indexArray, const uint32_t& numberOfIndices) {

	//	for (size_t i = 0; i < numberOfVertices; i++) {

	//		m_Vertices.push_back(vertexArray[i]);

	//	}

	//	for (size_t i = 0; i < numberOfIndices; i++) {

	//		m_Indices.push_back(indexArray[i]);

	//	}

	//}

	void Mesh::InitVAO(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) {

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal" },
		};

		HR_CORE_INFO("vertices size: {0}", vertices->size());
		m_VBO.reset(VertexBuffer::Create(vertices, vertices->size()));
		m_VBO->SetLayout(layout);

		HR_CORE_INFO("indices size: {0}", indices->size());
		m_IBO.reset(IndexBuffer::Create(indices, indices->size()));

		m_VAO.reset(VertexArray::Create());
		m_VAO->AddVertexBuffer(m_VBO);
		m_VAO->SetIndexBuffer(m_IBO);

	}

	//void Mesh::InitModelMatrix() {

	//	m_Position = glm::vec3(0.0f);
	//	m_Rotation = glm::vec3(0.0f);
	//	m_Scale = glm::vec3(1.0f);

	//	m_ModelMatrix = glm::mat4(1.0f);

	//	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	//	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	//	m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);

	//}

	//void Mesh::UpdateUniforms(Ref<Shader> shader) {

	//	std::dynamic_pointer_cast<Hildur::OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", m_ModelMatrix);

	//}


}