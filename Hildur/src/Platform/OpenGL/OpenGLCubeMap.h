#pragma once

#include "Hildur/Resource/CubeMap.h"


namespace Hildur {


	class OpenGLCubeMap : public CubeMap
	{
	public:

		OpenGLCubeMap(std::vector<std::string> paths);
		virtual ~OpenGLCubeMap();

		void GenerateIrradiance(uint32_t map) override;
		
		void Bind(uint32_t textureUnit = 0) override;

		void Enable() override;

		uint32_t GetWidth() override { return m_Width; }
		uint32_t GetHeight() override { return m_Height; }

		bool HasMipMaps() override { return m_IsMip; }

	private:

		void LoadCubeMap(std::vector<std::string> paths);

	private:

		uint32_t m_RendererID;
		uint32_t m_Width;
		uint32_t m_Height;

		bool m_IsMip;
	};


}