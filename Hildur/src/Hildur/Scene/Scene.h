#pragma once

#include "Hildur/Scene/GameObject.h"

#include "Hildur/ECS/quick.h"

#include "Hildur/Renderer/Mesh.h"


class RootNode {

public:

	RootNode();
	~RootNode();

	void AddGameObject(Hildur::Ref<Hildur::GameObject> gameObject);
	Hildur::Ref<Hildur::GameObject> GetGameObject(const uint32_t id);

	Hildur::Ref<Hildur::Mesh> GetMesh(const uint32_t id);

protected:

	std::unordered_map<uint32_t, Hildur::Ref<Hildur::GameObject>> m_Child;
	std::unordered_map<uint32_t, Hildur::Ref<Hildur::Mesh>> m_Meshes;

};

namespace Hildur {


	class Scene {

	public:

		Scene();
		~Scene();

		static Ref<Scene> Create();

		void AddGameObject(Ref<GameObject> gameObject);
		void AddGameObject(Ref<GameObject> gameObject, const uint32_t parent);

		Ref<GameObject> GetGameObject(const uint32_t id);
		Ref<Mesh> GetMesh(const uint32_t id);

		uint16_t GetID() { return ID; }

	private:

		//GameObject tree and Entity Component System
		RootNode m_RootNode;
		ECS::EntityComponentSystem ECS;


		uint16_t ID;
		static uint16_t s_NextID;

		static Ref<Scene> s_Scene;
		//static uint16_t s_SceneID;

	};


}