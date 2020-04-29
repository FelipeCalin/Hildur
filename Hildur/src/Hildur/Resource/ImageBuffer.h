#pragma once


namespace Hildur {


	class ImageBuffer
	{
	public:

		virtual void Enable() = 0;
		virtual void Bind(uint32_t textureUnit) = 0;
	};


}