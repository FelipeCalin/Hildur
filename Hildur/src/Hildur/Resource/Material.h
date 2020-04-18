#pragma once

#include "Hildur/Resource/Texture.h"
#include "Hildur/Resource/Shader.h"

#include <glm/glm.hpp>


namespace Hildur {


	class Material 
	{
	public:

		Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Ref<Texture> diffuseTex, Ref<Texture> specularTex);
		~Material();

		static Ref<Material> Material::Create(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Ref<Texture> diffuseTex, Ref<Texture> specularTex);

		void SendToShader(Ref<Shader> shader);

	private:

		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		Ref<Texture> m_DiffuseTex;
		Ref<Texture> m_SpecularTex;
	};


}