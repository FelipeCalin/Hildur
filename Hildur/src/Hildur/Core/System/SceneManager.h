#pragma once

//#include "PhotonBox/resource/Scene.h"
//#include "PhotonBox/core/ISystem.h"

#include "Hildur/Resource/Scene.h"
#include "Hildur/Core/ISystem.h"

#include <map>


class Scene;


namespace Hildur {


	class SceneManager : public ISystem {

	public:

		static void AddScene(const std::string name, Scene* scene);
		static void LoadScene(const std::string& name);
		static void UnloadScene(Scene* scene);
		static void UnloadScene(const std::string& name);
		static std::string GetCurrentName();
		static std::string GetQueuedName();
		static Scene* GetCurrentScene();
		static void SetSceneMap(std::map<std::string, Scene*>& sceneMap);

		void Start() override;
		void Destroy() override;

		void LoadSceneImmediately(const std::string& name);
		void LoadQueuedScene();
		bool SceneQueued() { return m_InQueue; }
		void DrawSceneList();
	private:

		static std::string m_NewScene;
		static std::string m_CurrentSceneName;
		static bool m_InQueue;
		static Scene* m_CurrentScene;
		static std::map<std::string, Scene*> m_SceneMap;

	};


}