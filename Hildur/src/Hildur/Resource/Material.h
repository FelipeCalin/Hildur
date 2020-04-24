#pragma once

#include "Hildur/Core/ManagedResource.h"

#include "Hildur/Resource/Texture.h"
#include "Hildur/Resource/Shader.h"
#include "Hildur/Resource/ImageBuffer.h"

#include <glm/glm.hpp>


namespace Hildur {


	class Material : public ManagedResource
	{
	public:

		//Material() {}
		Material(Ref<Shader> shader) : m_Shader(shader) {}
		~Material();

		template<typename T>
		void SetProperty(std::string name, T value)
		{
			if (m_UniformMap.find(name) != m_UniformMap.end())
			{
				((BaseObject<T>*)(m_UniformMap[name]))->m_Value = value;
			}
			else
			{
				m_UniformMap[name] = new BaseObject<T>(value, name);
			}
		}

		void SetImageBuffer(const std::string& uniformName, ImageBuffer* image);

		void UpdateUniforms();
		void UpdateUniforms(Ref<Shader> shader);
		void BindTextures();
		void BindTextures(Ref<Shader> customShader);
		Ref<Shader> GetShader();

		template <typename T>
		T GetProperty(const std::string& uniformName)
		{
			return ((BaseObject<T>*)(m_UniformMap[uniformName]))->m_Value;
		}

		static Ref<Material> Create(Ref<Shader> shader)
		{
			return CreateRef<Material>(shader);
		}

	private:

		struct SuperObject
		{
			virtual void Update(Ref<Shader> shader) = 0;
			virtual ~SuperObject() {}
		};

		template<class T>
		struct BaseObject : SuperObject
		{
			std::string m_Name;
			T m_Value;
			BaseObject(T value, std::string name) : m_Value(value), m_Name(name) {}
			void Update(Ref<Shader> shader) { shader->SetUniform(m_Name, m_Value); }
		};

		Ref<Shader> m_Shader;
		std::unordered_map<std::string, ImageBuffer*> m_ImageBufferMap;
		std::unordered_map<std::string, SuperObject*> m_UniformMap;
	};


}