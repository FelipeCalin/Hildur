#include "hrpcheaders.h"
#include "ResourceManager.h"

//#include "PhotonBox/core/LazyLoadable.h"
//#include "PhotonBox/core/system/DebugGUI.h"
//#include "PhotonBox/util/Logger.h"
//#include "PhotonBox/core/ManagedResource.h"

#include <imgui.h>

#include <chrono>
#include <algorithm>
#include <string>


namespace Hildur {


	std::map<std::string, ResourceManager::CountedResource> ResourceManager::m_ResourceMap;
	std::vector<LazyLoadable*> ResourceManager::m_InitializationList;
	std::vector<LazyLoadable*> ResourceManager::m_ReadyList;
	unsigned int ResourceManager::max_loadtime = 300000;

	bool ResourceManager::IsCompleted() 
	{
		return m_InitializationList.empty() && m_ReadyList.empty();
	}

	void ResourceManager::Load(bool lazy) 
	{
		if (IsCompleted()) return;

		ImGui::Begin("Assets Loader");
		ImGui::Text(("Initializing " + std::to_string(m_ReadyList.size())).c_str());
		for (int i = 0; i < m_ReadyList.size(); ++i) 
		{
			ImGui::TextUnformatted((std::to_string(i + 1) + " " + m_ReadyList[i]->GetFilePath()).c_str());
		}

		ImGui::Separator();
		ImGui::Text(("Loading " + std::to_string(m_InitializationList.size())).c_str());
		for (int i = 0; i < m_InitializationList.size(); ++i) 
		{
			ImGui::TextUnformatted((std::to_string(i + 1) + " " + m_InitializationList[i]->GetFilePath()).c_str());
		}
		ImGui::End();

		auto start = std::chrono::system_clock::now();

		while (!IsCompleted()) 
		{
			if (m_ReadyList.empty()) 
			{
				m_ReadyList.clear();
				for (int i = (int)m_InitializationList.size() - 1; i >= 0; --i) 
				{
					if (m_InitializationList[i]->GetState() == LazyLoadable::State::LOADED ||
						m_InitializationList[i]->GetState() == LazyLoadable::State::FAILED) 
					{
						if (m_InitializationList[i]->GetState() == LazyLoadable::State::LOADED) 
						{
							m_ReadyList.push_back(m_InitializationList[i]);
						}
						m_InitializationList.erase(m_InitializationList.begin() + i);
					}
				}
			}

			for (int i = (int)m_ReadyList.size() - 1; i >= 0; --i) 
			{
				m_ReadyList[i]->Initialize();
				m_ReadyList.erase(m_ReadyList.begin() + i);

				auto check = std::chrono::system_clock::now();
				if ((check - start).count() > max_loadtime&& lazy) return;
			}

			if (lazy) return;

		}

	}

	void ResourceManager::AddToInitializationList(LazyLoadable* resource) 
	{
		m_InitializationList.push_back(resource);
	}

	void ResourceManager::Reset() 
	{
		m_InitializationList.clear();
		m_ReadyList.clear();
	}

	void ResourceManager::Destroy() 
	{
		for (std::map<std::string, CountedResource>::iterator it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it) 
		{
			LazyLoadable* f = dynamic_cast<LazyLoadable*>(it->second.managedResource);
			if (f != nullptr) 
			{
				if (f->GetState() == LazyLoadable::State::LOADING) 
				{
					f->SetRogue();
				}
				else 
				{
					delete (it->second.managedResource);
				}

			}
			else 
			{
				delete (it->second.managedResource);
			}
		}
		m_ResourceMap.clear();
	}

	void ResourceManager::DeleteResource(Filepath path) 
	{
		DeleteResource(path.getAbsolutePath());
	}

	void ResourceManager::DeleteResource(std::string name) 
	{
		if (m_ResourceMap.find(name) != m_ResourceMap.end()) 
		{
			CountedResource* cr = &m_ResourceMap[name];
			if (cr->count <= 1) 
			{
				LazyLoadable* f = dynamic_cast<LazyLoadable*>(cr->managedResource);
				if (f != nullptr) 
				{
					if (f->GetState() == LazyLoadable::State::LOADING) 
					{
						f->SetRogue();
					}
					else 
					{
						delete (cr->managedResource);
					}
				}
				else 
				{
					delete (cr->managedResource);
				}

				m_ResourceMap.erase(name);
			}
			else 
			{
				--cr->count;
			}
		}
	}


}