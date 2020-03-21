#include "hrpcheaders.h"
#include "Entity.h"

#include <iostream>


namespace Hildur {


	int Entity::s_IdCnt = 0;

	Entity::Entity(Scene& scene)
	{
		Entity(scene, "Entity");
	}

	Entity::Entity(Scene& scene, std::string name)
	{
		m_ComponentMap = std::unordered_map<std::type_index, Component*>();
		m_Name = name;
		m_Id = Entity::s_IdCnt++;
		m_IsEnabled = true;
		m_IsStatic = true;
		m_Transform = addComponent<Transform>();
	}

	void Entity::destroyComponents()
	{
		for (std::unordered_map<std::type_index, Component*>::iterator it = m_ComponentMap.begin(); it != m_ComponentMap.end();)
		{
			Component* c = m_ComponentMap[(it->first)];
			c->destroy();
			it = m_ComponentMap.erase(it);
			delete c;
		}
	}

	void Entity::destroy()
	{
		std::vector<Transform*> children = m_Transform->getChildren();
		for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
		{
			(*it)->getEntity()->destroy();
		}

		destroyComponents();
		//parentScene->removeFromList(this); //TODO: FIX!!
	}

	int Entity::getId()
	{
		return m_Id;
	}

	void Entity::setEnable(bool enable)
	{
		for (std::unordered_map<std::type_index, Component*>::const_iterator it = m_ComponentMap.begin(); it != m_ComponentMap.end(); ++it)
		{
			it->second->setEnable(enable);
		}

		std::vector<Transform*> children = m_Transform->getChildren();
		for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
		{
			(*it)->getEntity()->setEnable(enable);
		}

		m_IsEnabled = enable;
	}

	void Entity::setStatic(bool _static)
	{
		m_IsStatic = _static;
	}


}