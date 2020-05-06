#include "hrpcheaders.h"
#include "MeshRenderer.h"

#include "Hildur/Component/Transform.h"
#include "Hildur/Component/LightEmitter.h"
#include "Hildur/Component/DirectionalLight.h"
#include "Hildur/Component/PointLight.h"
#include "Hildur/Renderer/RenderCommand.h"


namespace Hildur {


	void MeshRenderer::Render()
	{
		std::vector<LightEmitter*> nullList;
		Render(nullList, nullptr);
	}

	void MeshRenderer::Render(Ref<Shader> shader)
	{
		std::vector<LightEmitter*> nullList;
		Render(nullList, shader);
	}

	void MeshRenderer::Render(std::vector<LightEmitter*>& lights, Ref<Shader> shader)
	{
		if (shader == nullptr) 
			shader = m_Material->GetShader();

		shader->Bind();
		m_Mesh->GetVertexArray()->Bind();

		m_Material->SetProperty("u_ViewPos", Renderer::GetCameraPos());
		m_Material->SetProperty("u_ViewProjectionMat", Renderer::GetViewProjectionMat());
		m_Material->SetProperty("u_ModelMat", GetTransform()->GetTransformationMatrix());
		

		uint32_t nrOfDirLights = 0;
		uint32_t nrOfPointLights = 0;
		uint32_t nrOfSpotLights = 0;

		for (LightEmitter* light : lights)
		{
			if (light->GetEnable())
			{
				switch (light->GetType())
				{
					case LightEmitter::LE_DIRECTIONAL:
					{
						m_Material->SetProperty(("u_DirLights[" + std::to_string(nrOfDirLights) + "].direction"),
							light->GetTransform()->Forward());
						m_Material->SetProperty(("u_DirLights[" + std::to_string(nrOfDirLights) + "].ambient"),
							light->GetIntensity() * light->GetColor() * glm::vec3(0.2f));
						m_Material->SetProperty(("u_DirLights[" + std::to_string(nrOfDirLights) + "].diffuse"),
							light->GetIntensity() * light->GetColor() * glm::vec3(0.5f));
						m_Material->SetProperty(("u_DirLights[" + std::to_string(nrOfDirLights) + "].specular"),
							light->GetIntensity() * light->GetColor());

						nrOfDirLights++;
						break;
					}

					case LightEmitter::LE_POINT:
					{
						PointLight* PLight = (PointLight*)light;
						m_Material->SetProperty(("u_PointLights[" + std::to_string(nrOfPointLights) + "].position"),
							PLight->GetTransform()->GetPosition());
						m_Material->SetProperty(("u_PointLights[" + std::to_string(nrOfPointLights) + "].ambient"),
							PLight->GetIntensity() * light->GetColor() * glm::vec3(0.2f));
						m_Material->SetProperty(("u_PointLights[" + std::to_string(nrOfPointLights) + "].diffuse"),
							PLight->GetIntensity() * light->GetColor() * glm::vec3(0.5f));
						m_Material->SetProperty(("u_PointLights[" + std::to_string(nrOfPointLights) + "].specular"),
							PLight->GetIntensity() * light->GetColor());
						m_Material->SetProperty(("u_PointLights[" + std::to_string(nrOfPointLights) + "].constant"),
							PLight->GetConstant());
						m_Material->SetProperty(("u_PointLights[" + std::to_string(nrOfPointLights) + "].linear"),
							PLight->GetLinear());
						m_Material->SetProperty(("u_PointLights[" + std::to_string(nrOfPointLights) + "].quadratic"),
							PLight->GetQuadratic());

						nrOfPointLights++;
						break;
					}

					case LightEmitter::LE_SPOT:
					{
						HR_CORE_ASSERT(false, "Only Directional and Point lights supported!")

						break;
					}

					default:	HR_CORE_ASSERT(false, "Only Directional and Point lights supported!")
				}
			}
		}

		m_Material->SetProperty("nrDirectionalLights", nrOfDirLights);
		m_Material->SetProperty("nrPointLights", nrOfPointLights);
		m_Material->SetProperty("nrSpotLights", nrOfSpotLights);
		
		
		m_Material->UpdateUniforms();
		m_Material->BindTextures();

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