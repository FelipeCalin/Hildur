#pragma once

#include "Hildur/Scene/GameObject.h"


namespace Hildur {


	class Scene {

	public:

		Scene();
		~Scene();

		void AddGameObject(const GameObject& gameObject);

		static void SetCurrentSceneID(uint16_t ID) { s_CurrentSceneID = ID; }
		static uint32_t GetCurrentSceneID() { return s_CurrentSceneID; }

		static void SetCurrentScene(const Scene& ID) { s_CurrentScene = ID; }
		static Scene GetCurrentScene() { return s_CurrentScene; }

		uint16_t GetID() { return ID; }

	private:

		uint16_t ID;

		std::vector<GameObject> m_Resources;

	private:

		static Scene s_CurrentScene;

		static uint16_t s_CurrentSceneID;
		static uint16_t s_NextID;

	};


}