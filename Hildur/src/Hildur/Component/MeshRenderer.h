#pragma once

#include "Hildur/Component/Renderable.h"


namespace Hildur {


	class MeshRenderer : public Renderable
	{
	public:

		//void Init() override;
		//void OnEnable() override;
		//virtual void OnDisable() override;
		//virtual void Destroy() override;

		void Render() override;
		void Render(Ref<Shader> shader) override;
		void Render(std::vector<LightEmitter*>& lights, Ref<Shader> shader = nullptr) override;

		void RenderBoundingBox() override;
		
		bool IsInView() override;
		
		Ref<Mesh> GetMesh() { return m_Mesh; }
		MeshRenderer& SetMesh(Ref<Mesh> mesh);
	
	private:

		Ref<Mesh> m_Mesh;
		Ref<VertexArray> m_BoundingBoxVA;

	};


}