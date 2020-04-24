#pragma once


namespace Hildur {


	class ImageBuffer
	{
	public:
		virtual void enable() = 0;
		virtual void bind(unsigned int textureUnit) = 0;
	};


}