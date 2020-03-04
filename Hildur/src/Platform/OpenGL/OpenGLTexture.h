#pragma once

#include "Hildur/Renderer/Texture.h"


namespace Hildur {


	class OpenGLTexture2D : public Texture2D {

	public:

		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const uint32_t width, const uint32_t height, const uint32_t channels);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;

		virtual uint32_t GetID() const override { return m_RendererID; };
		virtual uint32_t GetTextureUnit() const override { return m_TextureUnit; };

	private:

		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;

		uint32_t m_TextureUnit;

		static uint32_t m_NextUnit;

	};


}