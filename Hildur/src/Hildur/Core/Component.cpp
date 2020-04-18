#include "hrpcheaders.h"
#include "Component.h"


namespace Hildur {


	void Component::setEnable(bool enable)
	{
		if (enable)
			OnEnable();
		else
			OnDisable();
		m_IsEnabled = enable;
	}

	std::string Component::getName()
	{
		std::string s = typeid(*this).name();
		return s.substr(6);
	}

	bool Component::getEnable()
	{
		return m_IsEnabled;
	}

	Transform* Component::getTransform()
	{
		return transform;
	}

	Entity* Component::getEntity()
	{
		return entity;
	}

	void Component::OnEnable() 
	{
	}

	void Component::OnDisable() 
	{
	}

	void Component::destroy() 
	{
	}

	std::type_index Component::getBaseType() 
	{
		return typeid(Component);
	}

	Component::~Component()
	{
	}


}