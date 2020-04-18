#pragma once

#include "Hildur/Resource/Filepath.h"

#include <string>


namespace Hildur {


	class LazyLoadable 
	{
	public:

		enum State 
		{
			UNSET,
			LOADING,
			LOADED,
			FAILED,
			INITIALIZED
		};

		virtual ~LazyLoadable() {}

		void LoadAsync();
		void Initialize();
		void SetRogue();
		void ForceLoad();

		State GetState();
		bool IsLoaded();
		bool IsInitialized();

		virtual std::string GetFilePath();

	protected:

		virtual bool LoadFromFile() = 0;
		virtual void SubmitBuffer() = 0;
		virtual void BlankInitialize() {}

	private:

		void Load();

	protected:

		Filepath m_FilePath;

	private:

		State m_State = State::UNSET;
		bool m_Rogue = false;
	};


}