#include "hrpcheaders.h"
#include "Renderable.h"

#include "Hildur/Core/System/Renderer.h"


namespace Hildur {


	void Renderable::Init()
	{
		Renderer::AddToRenderQueue(this);
	}

	void Renderable::OnDestroy() {}

	BoundingSphere Renderable::GetBoundingSphere()
	{
		return BoundingSphere(glm::vec3(0.0f), 0.001f);
	}

	void Renderable::Destroy()
	{
		OnDestroy();
		Renderer::RemoveFromRenderQueue(this);
	}

	void Renderable::SetRenderType(RenderType type)
	{
		Renderer::RemoveFromRenderQueue(this);
		Renderer::AddToRenderQueue(this);
		m_Type = type;
	}



}