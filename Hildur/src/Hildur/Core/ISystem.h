#pragma once

#include "Hildur/Core/Config.h"


namespace Hildur {


	class ISystem 
	{
	public:
		
		virtual void Init(Config::Profile profile) {}
		virtual void Start() = 0;
		virtual void Reset() {}
		virtual void Destroy() = 0;
	
	};


}