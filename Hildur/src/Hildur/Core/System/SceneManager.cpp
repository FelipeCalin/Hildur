#include "hrpcheaders.h"
#include "SceneManager.h"

//#include "PhotonBox/resource/Scene.h"
//#include "PhotonBox/core/Display.h"
//#include "PhotonBox/core/system/DebugGUI.h"

#include <imgui.h>


namespace Hildur {


	bool SceneManager::m_InQueue = false;
	Scene* SceneManager::m_CurrentScene;
	std::string SceneManager::m_NewScene;
	std::map<std::string, Scene*> SceneManager::m_SceneMap;
	std::string SceneManager::m_CurrentSceneName;

	void SceneManager::SetSceneMap(std::map<std::string, Scene*>& sceneMap) {

		m_SceneMap = sceneMap;
		LoadScene(m_SceneMap.begin()->first);

	}

	void SceneManager::Start() {

		LoadQueuedScene();

	}

	void SceneManager::AddScene(const std::string name, Scene* scene) {

		m_SceneMap[name] = scene;

	}

	void SceneManager::LoadScene(const std::string& name) {

		m_NewScene = name;
		m_InQueue = true;

	}

	void SceneManager::LoadSceneImmediately(const std::string& name) {

		if (m_SceneMap[name] == nullptr) return;

		Scene* oldScene = m_CurrentScene;
		m_CurrentScene = m_SceneMap[name];
		m_CurrentSceneName = name;

		m_CurrentScene->Load();
		if (oldScene != nullptr) oldScene->Unload();

		//TODO: create a way to change app title
		//Display::setTitle(Config::profile.appName + " - " + name);
		m_InQueue = false;

	}

	void SceneManager::LoadQueuedScene() {

		LoadSceneImmediately(m_NewScene);
		m_NewScene = "";

	}

	void SceneManager::UnloadScene(Scene* scene) {

		if (scene == nullptr) return;

		scene->Unload();
		m_CurrentScene = nullptr;

	}

	void SceneManager::UnloadScene(const std::string& name) {

		UnloadScene(m_SceneMap[name]);

	}

	std::string SceneManager::GetCurrentName() {

		return m_CurrentSceneName;

	}

	std::string SceneManager::GetQueuedName() {

		return m_NewScene;

	}

	Scene* SceneManager::GetCurrentScene() {

		return m_CurrentScene;

	}

	void SceneManager::Destroy() {

		m_CurrentScene->Unload();
		for (auto const& scene : m_SceneMap) {

			delete scene.second;

		}
		m_SceneMap.clear();

	}

	void SceneManager::DrawSceneList() {

		ImGui::Begin("Scenes");
		for (auto const& scene : m_SceneMap) {

			if (ImGui::Button(scene.first.c_str())) {

				LoadScene(scene.first);

			}

		}

		ImGui::End();
	}


}