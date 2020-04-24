#include "hrpcheaders.h"
#include "Component.h"


namespace Hildur {


	void Component::SetEnable(bool enable)
	{
		if (enable)
			OnEnable();
		else
			OnDisable();
		m_IsEnabled = enable;
	}

	std::string Component::GetName()
	{
		std::string s = typeid(*this).name();
		return s.substr(6);
	}

	bool Component::GetEnable()
	{
		return m_IsEnabled;
	}

	Transform* Component::GetTransform()
	{
		return transform;
	}

	Entity* Component::GetEntity()
	{
		return entity;
	}

	void Component::OnEnable() 
	{
	}

	void Component::OnDisable() 
	{
	}

	void Component::Destroy() 
	{
	}

	std::type_index Component::GetBaseType() 
	{
		return typeid(Component);
	}

	Component::~Component()
	{
	}


}