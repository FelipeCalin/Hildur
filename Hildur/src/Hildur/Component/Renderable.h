#pragma once

#include "Hildur/Core/Component.h"

#include "Hildur/Core/System/Renderer.h"
#include "Hildur/Resource/Material.h"

#include "Hildur/Types/BoundingSphere.h"
#include "Hildur/Types/AABB.h"


namespace Hildur {


	class Renderable : public Component
	{
	public:

		virtual ~Renderable() {}

		virtual void Init();

		virtual void Render() = 0;
		virtual void Render(Ref<Shader> shader) {}
		virtual void Render(std::vector<LightEmitter*>& lights, Ref<Shader> shader = nullptr) {}

		virtual void RenderBoundingBox() = 0;

		float GetID() const { return m_ID; }
		void SetID(float id) { m_ID = id; }

		virtual void OnDestroy();
		virtual BoundingSphere GetBoundingSphere();

		virtual bool IsInView() = 0;

		void Destroy();
		RenderType GetType() { return m_Type; }
		void SetRenderType(RenderType type);

		void SetMaterial(Ref<Material> material) { m_Material = material; }
		Ref<Material> GetMaterial() { return m_Material; }

	protected:

		bool m_GetReflected = true;
		bool m_CastShadows = true;

		RenderType m_Type;
		Ref<Material> m_Material;

		Ref<BoundingSphere> m_BoundingSphere;
		Ref<AABB> m_AABB;

		float m_ID;
	};


}