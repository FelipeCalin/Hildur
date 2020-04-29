#pragma once

#include "Hildur/Core/Component.h"

#include "Hildur/Core/System/Renderer.h"
#include "Hildur/Resource/Material.h"
#include "Hildur/Types/BoundingBox.h"


namespace Hildur {


	class Renderable : public Component
	{
	public:

		virtual ~Renderable() {}

		virtual void Init();
		virtual void Render() = 0;
		virtual void Render(Ref<Shader> shader) {}
		virtual void Render(Ref<Shader> shader, Ref<LightEmitter> light) {}
		virtual void OnDestroy();
		virtual BoundingSphere GetBoundingSphere();

		void Destroy();
		RenderType GetType() { return m_Type; }
		void SetRenderType(RenderType type);

		void SetMaterial(Material* material) { m_Material = material; }
		Material* GetMaterial() { return m_Material; }

	protected:

		bool m_GetReflected = true;
		bool m_CastShadows = true;

		RenderType m_Type;
		Material* m_Material;
	};


}