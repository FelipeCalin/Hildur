#include "hrpcheaders.h"
#include "Scene.h"


namespace Hildur {


	Scene defaultScene;
	Scene Scene::s_CurrentScene = defaultScene;

	uint16_t Scene::s_CurrentSceneID = 0;
	uint16_t Scene::s_NextID = 0;

	
	Scene::Scene() {

		ID = s_NextID;
		s_NextID++;

	}

	Scene::~Scene() {

	

	}

	void Scene::AddGameObject(const GameObject& gameObject) {

		m_Resources.push_back(gameObject);

	}


}