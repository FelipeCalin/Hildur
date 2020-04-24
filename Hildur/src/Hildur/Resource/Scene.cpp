#include "hrpcheaders.h"
#include "Scene.h"

//#include "PhotonBox/core/Core.h"
//#include "PhotonBox/core/Entity.h"
//#include "PhotonBox/core/ManagedResource.h"
//#include "PhotonBox/core/system/SceneManager.h"

#include "Hildur/Core/ManagedResource.h"
#include "Hildur/Core/System/SceneManager.h"


namespace Hildur {


	void Scene::Unload()
	{
		OnUnload();
		for (std::vector<Entity*>::iterator it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
		{
			(*it)->DestroyComponents();
			delete (*it);
		}

		m_EntityList.clear();

		for (std::vector<std::string>::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it)
		{
			ResourceManager::DeleteResource(*it);
		}

		m_Resources.clear();
	}

	void Scene::OnUnload() {}

	Entity* Scene::instantiate(std::string name)
	{
		Entity* entity = new Entity(*this, name);
		AddToList(entity);
		entity->m_ParentScene = this;
		return entity;
	}

	Entity* Scene::GetObjectByName(std::string name)
	{
		std::vector<Entity*> entitys = SceneManager::GetCurrentScene()->m_EntityList;
		for (std::vector<Entity*>::iterator obj = entitys.begin(); obj != entitys.end(); ++obj)
		{
			if ((*obj)->m_Name.find(name) != std::string::npos || (*obj)->m_Name.compare(name) == 0)
			{
				return *obj;
			}
		}

		return nullptr;
	}

	void Scene::Destroy(Entity* go)
	{
		go->Destroy();
	}

	void Scene::AddToList(Entity* go)
	{
		m_EntityList.push_back(go);
	}

	void Scene::RemoveFromList(Entity* go)
	{
		m_EntityList.erase(std::remove(m_EntityList.begin(), m_EntityList.end(), go), m_EntityList.end());
		delete go;
	}

	std::vector<Entity*>& Scene::GetEntities()
	{
		return m_EntityList;
	}


}