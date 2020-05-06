#include "hrpcheaders.h"
#include "OpenGLCubeMap.h"

#include <stb_image.h>
#include <glad/glad.h>


namespace Hildur {


	OpenGLCubeMap::OpenGLCubeMap(std::vector<std::string> paths)
		:m_IsMip(false), m_Width(0), m_Height(0)
	{
		LoadCubeMap(paths);
	}

	OpenGLCubeMap::~OpenGLCubeMap()
	{
	}

	void OpenGLCubeMap::GenerateIrradiance(uint32_t map)
	{
	}

	void OpenGLCubeMap::Bind(uint32_t textureUnit)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	void OpenGLCubeMap::Enable()
	{
	}

	void OpenGLCubeMap::LoadCubeMap(std::vector<std::string> paths)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		int nrChannels;
		for (unsigned int i = 0; i < 6; i++)
		{
			stbi_set_flip_vertically_on_load(false);
			unsigned char* data = stbi_load(paths[i].c_str(), (int*)&m_Width, (int*)&m_Height, &nrChannels, 0);
			if (data)
			{
				uint32_t internalFormat;
				uint32_t DataFormat;
				if (nrChannels == 3)
				{
					internalFormat = GL_RGB8;
					DataFormat = GL_RGB;
				}
				else
					if (nrChannels == 4)
					{
						internalFormat = GL_RGBA8;
						DataFormat = GL_RGBA;
					}

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, internalFormat, m_Width, m_Height, 0, DataFormat, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				HR_CORE_ERROR("Cubemap texture failed to load at path: {0}", paths[i].c_str());
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

}