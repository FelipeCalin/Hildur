#pragma once

#include "Hildur/Core/Component.h"
#include "Hildur/Resource/Texture.h"

#include <glm/glm.hpp>


namespace Hildur {


	class Sprite : public Component
	{
	public:

		virtual void Init();

		virtual void SetColor(const glm::vec4& color);
		virtual void SetTexture(Ref<Texture2D> texture);

		virtual glm::vec4 GetColor() const { return m_Color; }
		virtual Ref<Texture2D> GetTexture() const { return m_Texture; }

		virtual void Destroy();

	private:

		glm::vec4 m_Color;
		Ref<Texture2D> m_Texture;
	};


}