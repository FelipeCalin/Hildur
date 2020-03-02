#include "hrpcheaders.h"
#include "Scene.h"


namespace Hildur {

	
	uint16_t Scene::s_NextID = 0;

	Ref<Scene> Scene::s_Scene = nullptr;
	//uint16_t Scene::s_SceneID = 0;

	Scene::Scene() {

		ID = s_NextID;
		s_NextID++;

	}

	Scene::~Scene() {

	

	}

	Ref<Scene> Scene::Create() {

		if (!s_Scene) {

			s_Scene = std::make_shared<Scene>();
			return s_Scene;

		}
		else {

			return std::make_shared<Scene>();

		}

	}

	void Scene::AddGameObject(Ref<GameObject> gameObject) {

		m_RootNode.AddGameObject(gameObject);

	}

	void Scene::AddGameObject(Ref<GameObject> gameObject, const uint32_t parent) {

		m_RootNode.GetGameObject(parent)->AddChild(gameObject);

	}

	Ref<GameObject> Scene::GetGameObject(const uint32_t id) {

		return m_RootNode.GetGameObject(id);

	}

	Ref<Mesh> Scene::GetMesh(const uint32_t id) {

		return m_RootNode.GetMesh(id);

	}


}

RootNode::RootNode() {

	

}

RootNode::~RootNode() {



}

void RootNode::AddGameObject(Hildur::Ref<Hildur::GameObject> gameObject) {

	m_Child[gameObject->GetID()] = gameObject;
	m_Meshes[gameObject->GetID()] = gameObject->GetMesh();

}

Hildur::Ref<Hildur::GameObject> RootNode::GetGameObject(const uint32_t id) {

	return m_Child[id];

}

Hildur::Ref<Hildur::Mesh> RootNode::GetMesh(const uint32_t id) {

	return m_Meshes[id];

}