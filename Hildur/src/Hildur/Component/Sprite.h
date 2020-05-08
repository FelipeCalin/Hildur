#pragma once

#include "Hildur/Core/Component.h"
#include "Hildur/Component/Renderable.h"
#include "Hildur/Resource/Texture.h"

#include <glm/glm.hpp>


namespace Hildur {


	class Sprite : public Renderable
	{
	public:

		virtual void Init() override;

		virtual void Render() override;

		virtual glm::vec4 GetColor() const { return m_Color; }
		virtual void SetColor(const glm::vec4& color);

		virtual Ref<Texture2D> GetTexture() const { return m_Texture; }
		virtual void SetTexture(Ref<Texture2D> texture);

		virtual void Destroy();

	private:

		glm::vec4 m_Color;
		Ref<Texture2D> m_Texture;
	};


}