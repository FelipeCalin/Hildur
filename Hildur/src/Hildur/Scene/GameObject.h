#pragma once


namespace Hildur {


	class GameObject {

	public:

		GameObject();
		~GameObject();

		uint16_t ID = 0;

	protected:

		static uint16_t s_NextID;

	};


}