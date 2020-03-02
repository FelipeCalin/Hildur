#include "hrpcheaders.h"
#include "GameObject.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Hildur {


	uint16_t GameObject::s_NextID = 0;

	std::unordered_map<uint32_t, Ref<GameObject>> GameObject::m_GameObjects;
	std::unordered_map<uint32_t, Ref<GameObject>> GameObject::m_Child;

	GameObject::GameObject(const std::string name, Ref<Mesh> mesh)
		: m_Name(name), m_Mesh(mesh) {

		//Set unique GameObject ID
		m_ID = s_NextID++;

		//Initialize model matrix
		m_Position = glm::vec3(0.0f);
		m_Rotation = glm::vec3(0.0f);
		m_Scale = glm::vec3(1.0f);

		m_ModelMatrix = glm::mat4(1.0f);

		m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);

	}

	GameObject::~GameObject() {



	}

	Ref<GameObject> GameObject::Create(const std::string name, Ref<Mesh> mesh) {

		//Set unique GameObject ID
		uint32_t ID = s_NextID;

		//Add to GameObject list
		m_GameObjects[ID] = std::make_shared<GameObject>(name, mesh);

		return m_GameObjects[ID];

	}

	void GameObject::Update(Ref<Shader> shader) {

		m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);

		std::dynamic_pointer_cast<Hildur::OpenGLShader>(shader)->UploadUniformMat4("u_ModelM", m_ModelMatrix);

	}

	void GameObject::AddChild(Ref<GameObject> gameObject) {

		m_Child[gameObject->GetID()] = gameObject;

	}


}