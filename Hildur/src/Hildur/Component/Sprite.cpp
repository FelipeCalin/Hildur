#include "hrpcheaders.h"
#include "Sprite.h"

#include "Hildur/Core/System/Renderer2D.h"
#include "Hildur/Renderer/RenderCommand.h"

#include "Hildur/Component/Transform.h"


namespace Hildur {


	void Sprite::Init()
	{
		m_Color = glm::vec4(1.0f);
		m_Texture = nullptr;

		Renderer2D::AddToRenderQueue(this);
	}

	void Sprite::Render()
	{
		HR_PROFILE_FUNCTION()

		Renderer2D::DrawRotatedQuad(GetTransform()->GetPosition(), GetTransform()->GetScale(), GetTransform()->GetRotation().z, m_Color);
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