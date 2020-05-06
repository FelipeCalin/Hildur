#pragma once

#include "Hildur/Core/ManagedResource.h"
#include "Hildur/Resource/ImageBuffer.h"
#include "Hildur/Resource/Filepath.h"


namespace Hildur {


	class CubeMap : public ManagedResource, public ImageBuffer
	{
	public:

		enum Face
		{
			RIGHT,
			LEFT,
			TOP,
			BOTTOM,
			BACK,
			FRONT
		};

		//CubeMap(std::vector<std::string> paths);
		virtual ~CubeMap() = default;

		virtual void GenerateIrradiance(uint32_t map) = 0;

		virtual void Bind(uint32_t textureUnit = 0) = 0;

		virtual void Enable() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual bool HasMipMaps() = 0;

		static Ref<CubeMap> Create(std::vector<std::string> paths);

	};


}