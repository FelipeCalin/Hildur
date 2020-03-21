#pragma once

#include "Hildur/Component/Transform.h"
#include "Hildur/Core/Component.h"

#include "Hildur/Util/Log.h"

#include <unordered_map>
#include <typeindex>
#include <iostream>


namespace Hildur {


	class Scene;

	class Entity {

	public:

		Entity(Scene& scene);
		Entity(Scene& scene, std::string name);

		template <class T>
		T* addComponent() {

			if (m_ComponentMap.find(typeid(T)) != m_ComponentMap.end()) {

				HR_CORE_ERROR("{0} Entity already contains Component", m_Name);
				return nullptr;

			}

			T* c = new T();
			c->setup(this, m_Transform);

			m_ComponentMap.insert(std::pair<std::type_index, Component*>(typeid(T), c));
			return c;

		}

		template<class T>
		T* getComponent() {

			if (m_ComponentMap.find(typeid(T)) == m_ComponentMap.end()) {

				for (std::unordered_map<std::type_index, Component*>::iterator it = m_ComponentMap.begin(); it != m_ComponentMap.end(); ++it) {

					if (typeid(T).hash_code() == it->second->getBaseType().hash_code()) {

						return (T*)it->second;

					}

				}

				HR_CORE_ERROR("Component {0} could not be found!", typeid(T).name());
				return nullptr;

			}

			return (T*)m_ComponentMap[typeid(T)];

		}

		template <class T>
		void removeComponent() {

			T* c = getComponent<T>();
			m_ComponentMap.erase(typeid(T));
			delete c;

		}

		void removeComponent(std::type_index ti) {

			Component* c = m_ComponentMap[ti];
			c->destroy();
			m_ComponentMap.erase(ti);
			delete c;

		}

		void destroyComponents();
		void destroy();
		int getId();
		void setEnable(bool enable);
		bool getEnable() { return m_IsEnabled; }
		void setStatic(bool _static);
		bool getStatic() { return m_IsStatic; }

	public:

		Scene* m_ParentScene;
		Transform* m_Transform;
		std::string m_Name;

	private:

		static int s_IdCnt;
		bool m_IsEnabled;
		bool m_IsStatic;
		int m_Id;
		std::unordered_map<std::type_index, Component*> m_ComponentMap;

	};


}