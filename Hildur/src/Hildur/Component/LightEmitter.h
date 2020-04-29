#pragma once

#include "Hildur/Core/Component.h"

#include <glm/glm.hpp>


namespace Hildur {


	class Shader;

	class LightEmitter : public Component
	{
	public:

		virtual ~LightEmitter() {}

		glm::vec3 GetColor() const { return m_Color; }
		void SetColor(const glm::vec3& color) { m_Color = color; }

		float GetIntensity() const { return m_Intensity; }
		void SetIntensity(float intensity) { m_Intensity = intensity; }

		Ref<Shader> GetLightShader() const { return m_LightShader; }
		void SetLightShader(Ref<Shader> shader) { m_LightShader = shader; }

		std::type_index GetBaseType() override { return typeid(LightEmitter); }

	protected:

		glm::vec3 m_Color = glm::vec3(1.0f);
		float m_Intensity = 1;

		Ref<Shader> m_LightShader;
	};


}