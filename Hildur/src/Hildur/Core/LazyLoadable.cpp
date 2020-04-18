#include "hrpcheaders.h"
#include "Hildur/Core/LazyLoadable.h"

#include "Hildur/Core/System/ResourceManager.h"

#include <thread>
#include <iostream>


namespace Hildur {


	void LazyLoadable::LoadAsync()
	{
		BlankInitialize();

		ResourceManager::AddToInitializationList(this);
		m_State = State::LOADING;
		std::thread{ &LazyLoadable::Load, this }.detach();
	}

	void LazyLoadable::ForceLoad()
	{
		BlankInitialize();
		Load();
		Initialize();
	}

	void LazyLoadable::SetRogue() 
	{
		m_Rogue = true;
	}

	void LazyLoadable::Initialize() 
	{
		if (m_State != State::FAILED) 
		{
			SubmitBuffer();
			m_State = State::INITIALIZED;
		}
	}

	LazyLoadable::State LazyLoadable::GetState() 
	{
		return m_State;
	}

	bool LazyLoadable::IsLoaded() 
	{
		return m_State == State::LOADED;
	}

	bool LazyLoadable::IsInitialized()
	{
		return m_State == State::INITIALIZED;
	}

	std::string LazyLoadable::GetFilePath()
	{
		return m_FilePath.getAbsolutePath();
	}

	void LazyLoadable::Load() 
	{
		m_State = LoadFromFile() ? State::LOADED : State::FAILED;

		if (m_Rogue)
		{
			delete this;
		}
	}


}