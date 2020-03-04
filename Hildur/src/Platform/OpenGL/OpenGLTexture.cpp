#include "hrpcheaders.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

#include <glad/glad.h>


namespace Hildur {


	uint32_t OpenGLTexture2D::m_NextUnit = 1;

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) {


		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		HR_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		m_TextureUnit = 0;


		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 3) {

			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;

		}
		else if (channels == 4) {

			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;

		}

		HR_CORE_ASSERT(internalFormat & dataFormat, "Data format not supported!");

		
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		stbi_image_free(data);


		m_TextureUnit = m_NextUnit++;

	}

	OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height, const uint32_t channels) {

		m_Width = width;
		m_Height = height;

		m_TextureUnit = 0;


		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 3) {

			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;

		}
		else if (channels == 4) {

			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;

		}

		HR_CORE_ASSERT(internalFormat & dataFormat, "Data format not supported!");


		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);


		m_TextureUnit = m_NextUnit++;

	}

	OpenGLTexture2D::~OpenGLTexture2D() {

		glDeleteTextures(1, &m_RendererID);

	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {

		if (slot != 0) {
			glActiveTexture((int)GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
		} 
		else {

			glActiveTexture((int)GL_TEXTURE0 + m_TextureUnit);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

		}

	}


}