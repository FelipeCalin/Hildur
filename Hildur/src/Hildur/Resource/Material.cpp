#include "hrpcheaders.h"
#include "Material.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Hildur {


	Material::Material(Ref<Shader> shader)
		: m_Shader(shader)
	{
	}

	Material::~Material()
	{
		for (std::unordered_map<std::string, SuperObject*>::iterator it = m_UniformMap.begin(); it != m_UniformMap.end(); ++it)
		{
			delete it->second;
		}
		m_UniformMap.clear();
	}

	void Material::SetImageBuffer(const std::string& uniformName, ImageBuffer* image)
	{
		m_ImageBufferMap[uniformName] = image;
	}

	void Material::UpdateUniforms()
	{
		UpdateUniforms(m_Shader);
	}

	void Material::UpdateUniforms(Ref<Shader> shader)
	{
		for (std::unordered_map<std::string, SuperObject*>::const_iterator it = m_UniformMap.begin(); it != m_UniformMap.end(); ++it)
		{
			it->second->Update(shader);
		}
	}

	void Material::BindTextures()
	{
		BindTextures(m_Shader);
	}

	void Material::BindTextures(Ref<Shader> shader)
	{
		// TODO: Fix
		//for (std::unordered_map<std::string, ImageBuffer*>::const_iterator it = m_ImageBufferMap.begin(); it != m_ImageBufferMap.end(); ++it)
		//{
		//	if (shader->textures.find(it->first) != shader->textures.end())
		//		it->second->bind(shader->textures[it->first].unit);
		//}
	}

	Ref<Shader> Material::GetShader()
	{
		return m_Shader;
	}


}