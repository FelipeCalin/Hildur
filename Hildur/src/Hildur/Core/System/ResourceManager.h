#pragma once
#include <vector>
#include <map>

#include "Hildur/Resource/Filepath.h"

#include "Hildur/Core/LazyLoadable.h"
#include "Hildur/Core/ManagedResource.h"


class LazyLoadable;
class ManagedResource;


namespace Hildur {


	class ResourceManager {

	public:

		static unsigned int max_loadtime;

		void Load(bool lazy = true);
		void Reset();
		void Destroy();

		template <class T, typename D>
		static T* CreateResource(std::string name, D data) {

			//TODO: create hildur static assertions
			static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

			T* resource;

			if (m_ResourceMap.find(name) == m_ResourceMap.end()) {

				resource = new T(data);
				m_ResourceMap[name] = { 1, resource };

			}
			else {

				CountedResource* cr = &m_ResourceMap[name];
				++cr->count;
				resource = (T*)cr->managedResource;

			}

			return resource;

		}

		template <class T>
		static T* CreateResource(std::string name) {

			static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

			T* resource;

			if (m_ResourceMap.find(name) == m_ResourceMap.end()) {

				resource = new T();
				m_ResourceMap[name] = { 1, resource };

			}
			else {

				CountedResource* cr = &m_ResourceMap[name];
				++cr->count;
				resource = (T*)cr->managedResource;

			}

			return resource;

		}

		template <class T, typename C>
		static T* CreateResource(Filepath path, C config) {

			static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

			T* resource;

			if (m_ResourceMap.find(path.getAbsolutePath()) == m_ResourceMap.end()) {

				resource = new T(path, config);
				m_ResourceMap[path.getAbsolutePath()] = { 1, resource };

			}
			else {

				CountedResource* cr = &m_ResourceMap[path.getAbsolutePath()];
				++cr->count;
				resource = (T*)cr->managedResource;

			}

			return resource;

		}

		template <class T>
		static T* CreateResource(Filepath path) {

			static_assert(std::is_base_of<ManagedResource, T>::value, "T must inherit from ManagedResource");

			T* resource;

			if (m_ResourceMap.find(path.getAbsolutePath()) == m_ResourceMap.end()) {

				resource = new T(path);
				m_ResourceMap[path.getAbsolutePath()] = { 1, resource };

			}
			else {

				CountedResource* cr = &m_ResourceMap[path.getAbsolutePath()];
				++cr->count;
				resource = (T*)cr->managedResource;

			}

			return resource;

		}

		static void DeleteResource(Filepath path);
		static void DeleteResource(std::string name);

		static bool IsCompleted();
		static void AddToInitializationList(LazyLoadable* resource);

	private:

		struct CountedResource {

			int count;
			ManagedResource* managedResource;

		};

		static std::map<std::string, CountedResource> m_ResourceMap;
		static std::vector<LazyLoadable*> m_InitializationList;
		static std::vector<LazyLoadable*> m_ReadyList;

	};


}