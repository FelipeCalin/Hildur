#include "hrpcheaders.h"
#include "Sprite.h"

#include "Hildur/Core/System/Renderer2D.h"


namespace Hildur {


	void Sprite::Init()
	{
		m_Color = glm::vec4(1.0f);
		m_Texture = nullptr;

		Renderer2D::AddToRenderQueue(this);
	}

	void Sprite::SetColor(const glm::vec4& color)
	{
		
	}

	void Sprite::SetTexture(Ref<Texture2D> texture)
	{

	}

	void Sprite::Destroy()
	{
		Renderer2D::RemoveFromRenderQueue(this);
	}


}