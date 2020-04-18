#include "hrpcheaders.h"
#include "Material.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Hildur {


	Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Ref<Texture> diffuseTex, Ref<Texture> specularTex)
		: m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_DiffuseTex(diffuseTex), m_SpecularTex(specularTex) 
	{
	}

	Material::~Material() 
	{
	}

	Ref<Material> Material::Create(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Ref<Texture> diffuseTex, Ref<Texture> specularTex) 
	{
		return CreateRef<Material>(ambient, diffuse, specular, diffuseTex, specularTex);
	}

	void Material::SendToShader(Ref<Shader> shader) 
	{
		shader->Bind();

		std::dynamic_pointer_cast<Hildur::OpenGLShader>(shader)->UploadUniformFloat3("material.ambient", m_Ambient);
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(shader)->UploadUniformFloat3("material.diffuse", m_Diffuse);
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(shader)->UploadUniformFloat3("material.specular", m_Specular);
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(shader)->UploadUniformInt("diffuseTex", 0);
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(shader)->UploadUniformInt("specularTex", 1);

		shader->UnBind();
	}


}