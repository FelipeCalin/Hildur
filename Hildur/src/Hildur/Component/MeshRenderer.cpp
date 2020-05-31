#include "hrpcheaders.h"
#include "MeshRenderer.h"

#include "Hildur/Component/Camera.h"

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
		{
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

					default:	HR_CORE_ASSERT(false, "Only Directional and Point lights supported!");
					}
				}
			}

			m_Material->SetProperty("nrDirectionalLights", nrOfDirLights);
			m_Material->SetProperty("nrPointLights", nrOfPointLights);
			m_Material->SetProperty("nrSpotLights", nrOfSpotLights);


			m_Material->UpdateUniforms();
			m_Material->BindTextures();
		}
		else
		{
			shader->Bind();

			m_Mesh->GetVertexArray()->Bind();

			shader->SetFloat3("u_ViewPos", Renderer::GetCameraPos());
			shader->SetMat4("u_ViewProjectionMat", Renderer::GetViewProjectionMat());
			shader->SetMat4("u_ModelMat", GetTransform()->GetTransformationMatrix());
		}
		

		RenderCommand::DrawIndexed(m_Mesh->GetVertexArray());

		//Renderer::addDrawCall();
	}

	void MeshRenderer::RenderBoundingBox()
	{
		m_BoundingBoxVA->Bind();
	
		RenderCommand::DrawIndexed(m_BoundingBoxVA);
	}

	bool MeshRenderer::IsInView()
	{
		return Hildur::Camera::GetMainCamera()->FrustumTest(this);
	}

	MeshRenderer& MeshRenderer::SetMesh(Ref<Mesh> mesh)
	{
		m_Mesh = mesh;

		glm::vec3 min = m_Mesh->GetMinBoundingBox();
		glm::vec3 max = m_Mesh->GetMaxBoundingBox();

		float verts[8 * 3] =
		{
			min.x, min.y, min.z,
			min.x, min.y, max.z,
			min.x, max.y, min.z,
			min.x, max.y, max.z,
			max.x, min.y, min.z,
			max.x, min.y, max.z,
			max.x, max.y, min.z,
			max.x, max.y, max.z,
		};

		uint32_t indices[] =
		{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
		};

		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" }
		};

		Ref<VertexBuffer> vb = VertexBuffer::Create(verts, 8 * 3);
		Ref<IndexBuffer> ib = IndexBuffer::Create(indices, 8 * 3);
		vb->SetLayout(layout);
		m_BoundingBoxVA = VertexArray::Create();
		m_BoundingBoxVA->AddVertexBuffer(vb);
		m_BoundingBoxVA->SetIndexBuffer(ib);

		return *this;
	}


}