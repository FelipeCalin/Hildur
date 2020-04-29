#pragma once

#include "Hildur/Component/Renderable.h"


namespace Hildur {


	class MeshRenderer : public Renderable
	{
	public:

		//virtual void Init() override;
		//virtual void OnEnable() override;
		//virtual void OnDisable() override;
		//virtual void Destroy() override;

		void Render() override;
		void Render(Ref<Shader> shader) override;
		void Render(Ref<Shader> shader, Ref<LightEmitter> light) override;

		BoundingSphere GetBoundingSphere();
		Ref<Mesh> GetMesh() { return m_Mesh; }

		MeshRenderer& SetMesh(Ref<Mesh> mesh);
	
	private:

		Ref<Mesh> m_Mesh;
	};


}