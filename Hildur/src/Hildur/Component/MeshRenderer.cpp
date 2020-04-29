#include "hrpcheaders.h"
#include "MeshRenderer.h"

#include "Hildur/Renderer/RenderCommand.h"


namespace Hildur {


	void MeshRenderer::Render()
	{
		Render(nullptr, nullptr);
	}

	void MeshRenderer::Render(Ref<Shader> shader)
	{
		Render(shader, nullptr);
	}

	void MeshRenderer::Render(Ref<Shader> shader, Ref<LightEmitter> light)
	{
		if (shader == nullptr) shader = m_Material->GetShader();

		//glBindVertexArray(m_Mesh->GetVAO());
		shader->Bind();

		//if (light == nullptr)
		//	shader->update(transform);
		//else
		//	shader->update(transform, light);

		//shader->updateTextures();

		m_Material->UpdateUniforms(shader);
		m_Material->BindTextures(shader);

		//shader->enableAttributes();

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->getEBO());
		//glDrawElements(GL_TRIANGLES, (GLsizei)_mesh->indices.size(), GL_UNSIGNED_INT, 0);
		RenderCommand::DrawIndexed(m_Mesh->GetVertexArray());

		//Renderer::addDrawCall();

		//shader->disableAttributes();

		//glBindVertexArray(0);
	}

	BoundingSphere MeshRenderer::GetBoundingSphere()
	{
		//TODO: fix
		return BoundingSphere();
	}

	MeshRenderer& MeshRenderer::SetMesh(Ref<Mesh> mesh)
	{
		m_Mesh = mesh;
		return *this;
	}



}