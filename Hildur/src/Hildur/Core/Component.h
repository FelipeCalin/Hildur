#pragma once

#include <string>
#include <typeindex>


namespace Hildur {


	class Entity;
	class Transform;

	class Component 
	{
	public:

		void SetEnable(bool enable);
		std::string GetName();
		bool GetEnable();
		Transform* GetTransform();
		Entity* GetEntity();

		void Setup(Entity* entity, Transform* transform)
		{
			this->entity = entity;
			this->transform = transform;
			Init();
		}


		virtual ~Component() = 0;
		virtual void Init() {}
		virtual void OnEnable();
		virtual void OnDisable();
		virtual void Destroy();
		virtual void RenderInspector() {};
		virtual std::type_index GetBaseType();

	protected:

		Entity* entity;
		Transform* transform;

	private:

		bool m_IsEnabled = true;
	};


}