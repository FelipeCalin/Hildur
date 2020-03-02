#pragma once

#include "Hildur/Renderer/Shader.h"
#include "Hildur/Renderer/Mesh.h"

#include "Hildur/ECS/ECS.h"


namespace Hildur {


	class GameObject {

	public:

		GameObject(const std::string name, Ref<Mesh> mesh);
		~GameObject();

		static Ref<GameObject> Create(const std::string name, Ref<Mesh> mesh);

		void Update(Ref<Shader> shader);

		void AddChild(Ref<GameObject> gameObject);

		uint32_t GetID() const { return m_ID; }
		static Ref<GameObject> GetChild(uint32_t id) { return m_Child[id]; }

		Ref<Mesh> GetMesh() const { return m_Mesh; }

	public:

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

	private:

		const std::string& m_Name;
		uint16_t m_ID = 0;

		Ref<Mesh> m_Mesh;
		glm::mat4 m_ModelMatrix;

		static std::unordered_map<uint32_t, Ref<GameObject>> m_GameObjects;
		static std::unordered_map<uint32_t, Ref<GameObject>> m_Child;

	protected:

		static uint16_t s_NextID;

	};


}