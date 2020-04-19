#include "hrpcheaders.h"
#include "OpenGLTexture.h"

#include <stb_image.h>


namespace Hildur {


	OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height)
		: m_Width(width), m_Height(height)
	{
		HR_PROFILE_FUNCTION()

		m_InternalFomat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFomat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) 
	{
		HR_PROFILE_FUNCTION()

		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		stbi_uc* data = nullptr;
		{
			HR_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)")
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		HR_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 3) {

			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;

		}
		else if (channels == 4) {

			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;

		}

		m_InternalFomat = internalFormat;
		m_DataFormat = dataFormat;

		HR_CORE_ASSERT(internalFormat & dataFormat, "Data format not supported!");
		
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() 
	{
		HR_PROFILE_FUNCTION()

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) 
	{
		HR_PROFILE_FUNCTION()

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;

		HR_CORE_ASSERT(size == m_Width * m_Width * bpp, "Data must be entire texture!");

		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const 
	{
		HR_PROFILE_FUNCTION()

		glActiveTexture((int)GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}


}