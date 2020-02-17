#include "hrpcheaders.h"
#include "GameObject.h"


namespace Hildur {


	uint16_t GameObject::s_NextID = 0;

	GameObject::GameObject() {

		ID = s_NextID;
		s_NextID++;

	}

	GameObject::~GameObject() {



	}


}