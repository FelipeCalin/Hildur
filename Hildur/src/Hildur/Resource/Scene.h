#pragma once

//#include "PhotonBox/core/Entity.h"
//#include "PhotonBox/resource/CubeMap.h"
//#include "PhotonBox/resource/Mesh.h"
//#include "PhotonBox/resource/Texture.h"
//#include "PhotonBox/resource/Filepath.h"
//#include "PhotonBox/core/system/ResourceManager.h"

#include "Hildur/Core/Entity.h"
#include "Hildur/Core/System/ResourceManager.h"

#include "Hildur/Resource/Filepath.h"

#include <vector>
#include <map>
#include <type_traits>


class ManagedResource;


namespace Hildur {


	class Scene : ManagedResource
	{
	public:

		virtual void Load() = 0;
		virtual void OnUnload();
		void Unload();

		Entity* instantiate(std::string name);

		template <class T, typename D>
		T* CreateResource(std::string name, D data) 
		{
			static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

			m_Resources.push_back(name);

			return ResourceManager::CreateResource<T, D>(name, data);
		}

		template <class T>
		T* CreateResource(std::string name) 
		{
			static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

			m_Resources.push_back(name);

			return ResourceManager::CreateResource<T>(name);
		}

		template <class T, typename C>
		T* CreateResource(Filepath path, C config) 
		{
			static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

			m_Resources.push_back(path.getAbsolutePath());

			return ResourceManager::CreateResource<T, C>(path, config);
		}

		template <class T>
		T* CreateResource(Filepath path) 
		{
			static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

			m_Resources.push_back(path.getAbsolutePath());

			return ResourceManager::CreateResource<T>(path);
		}

		void Destroy(Entity* go);
		void RemoveFromList(Entity* go);
		Entity* GetObjectByName(std::string name);
		std::vector<Entity*>& GetEntities();

	private:

		void AddToList(Entity* go);

	private:

		std::vector<Entity*> m_EntityList;
		std::vector<std::string> m_Resources;
	};


}