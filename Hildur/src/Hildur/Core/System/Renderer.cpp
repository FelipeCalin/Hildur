  #include "hrpcheaders.h"
#include "Renderer.h"

#include "Hildur/Resource/Texture.h"
#include "Hildur/Component/Transform.h"
#include "Hildur/Component/Renderable.h" 
#include "Hildur/Component/MeshRenderer.h"
#include "Hildur/Component/LightEmitter.h"
#include "Hildur/Component/DirectionalLight.h"
#include "Hildur/Component/Camera.h"

#include "Hildur/Renderer/RenderCommand.h"
#include "Hildur/Core/System/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>


namespace Hildur {


	struct RendererStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static std::vector<Renderable*> s_RenderList;
	static std::unordered_map<uint32_t, Renderable*> s_ObjectIDList;
	static std::vector<LightEmitter*> s_LightsList;

	static RendererStorage* s_Data;

	static glm::mat4 s_ViewMat;
	static glm::mat4 s_ProjectionMat;
	static glm::mat4 s_ViewProjectionMat;
	static glm::vec3 s_CameraPosition;

	static Ref<CubeMap> s_SkyBox = nullptr;
	static Ref<Mesh> s_SkyboxMesh;
	static Ref<Shader> s_SkyboxShader;
	static Ref<Shader> s_IDShader;


	void Renderer::Init() 
	{
		HR_PROFILE_FUNCTION();
		
		RenderCommand::Init();
		Renderer2D::Init();

		std::vector<glm::vec3> skyboxVertices = {
			// positions          
			{-1.0f,  1.0f, -1.0f},
			{-1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},

			{-1.0f, -1.0f,  1.0f},
			{-1.0f, -1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{-1.0f,  1.0f,  1.0f},
			{-1.0f, -1.0f,  1.0f},

			{ 1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},

			{-1.0f, -1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f},
			{-1.0f, -1.0f,  1.0f},

			{-1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f, -1.0f},

			{-1.0f, -1.0f, -1.0f},
			{-1.0f, -1.0f,  1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{-1.0f, -1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f}
		};

		s_SkyboxMesh = Mesh::Create(skyboxVertices);
		s_SkyboxShader = Shader::Create("assets/shaders/Skybox.glsl");

		s_IDShader = Hildur::Shader::Create("assets/shaders/Color.glsl");
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		s_ViewMat = camera.GetViewMatrix();
		s_ProjectionMat = camera.GetProjectionMatrix();
		s_ViewProjectionMat = camera.GetViewProjectionMatrix();

		//TODO: Only update uniforms on active scene materials
	}

	void Renderer::BeginScene(PerspectiveCamera& camera) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		s_ViewMat = camera.GetViewMatrix();
		s_ProjectionMat = camera.GetProjectionMatrix();
		s_ViewProjectionMat = camera.GetViewProjectionMatrix();
		s_CameraPosition = camera.GetPosition();
	}

	void Renderer::EndScene()
	{
		HR_PROFILE_RENDERER_FUNCTION()

		//Render skybox
		if (s_SkyBox != nullptr)
		{
			s_SkyboxShader->Bind();
			s_SkyBox->Bind(4);
			s_SkyboxShader->SetInt("u_CubeMap", 4);
			s_SkyboxShader->SetMat4("u_ViewProjectionMat", s_ProjectionMat
				* glm::mat4(glm::mat3(s_ViewMat)));

			s_SkyboxMesh->GetVertexArray()->Bind();

			Hildur::RenderCommand::DrawIndexed(s_SkyboxMesh->GetVertexArray());
		}
	}

	void Renderer::Prep()
	{
		HR_PROFILE_RENDERER_FUNCTION()
	
		if (Camera::GetMainCamera())
		{
			s_ViewMat = Camera::GetMainCamera()->GetViewMatrix();
			s_ProjectionMat = Camera::GetMainCamera()->GetProjectionMatrix();
			s_ViewProjectionMat = Camera::GetMainCamera()->GetViewProjection();

			s_CameraPosition = Camera::GetMainCamera()->GetTransform()->GetPositionWorld();
		}
	}

	void Renderer::RenderQueue()
	{
		HR_PROFILE_RENDERER_FUNCTION()

		for (Renderable* renderable : s_RenderList)
		{
			if (renderable->GetEnable())
			{
				renderable->Render(s_LightsList);
			}
		}
	}

	void Renderer::End()
	{
		//Render Skybox

		if (s_SkyBox != nullptr)
		{
			s_SkyboxShader->Bind();
			s_SkyBox->Bind(1);
			s_SkyboxShader->SetInt("u_CubeMap", 1);
			s_SkyboxShader->SetMat4("u_ViewProjectionMat", s_ProjectionMat
				* glm::mat4(glm::mat3(s_ViewMat)));

			s_SkyboxMesh->GetVertexArray()->Bind();

			Hildur::RenderCommand::DrawIndexed(s_SkyboxMesh->GetVertexArray());
		}
	}

	void Renderer::RenderID()
	{
		HR_PROFILE_RENDERER_FUNCTION()

			for (Renderable* renderable : s_RenderList)
			{
				if (renderable->GetEnable())
				{
					s_IDShader->SetFloat4("u_Color", { renderable->GetID() * 0.1f, 0.0f, 0.0f, 1.0f });

					renderable->Render(s_IDShader);
				}
			}
	}
	
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformFloat3("u_ViewPos", s_CameraPosition);
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformMat4("u_ViewProjectionMat", s_ViewProjectionMat);
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformMat4("u_ModelMat", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformMat4("u_ViewProjectionMat", s_ViewProjectionMat);
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformMat4("u_ModelMat", transform);

		mesh->GetVertexArray()->Bind();

		RenderCommand::DrawIndexed(mesh->GetVertexArray());
	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform)
	{
		HR_PROFILE_RENDERER_FUNCTION()

		Ref<Shader> shader = material->GetShader();

		shader->Bind();
		material->UpdateUniforms();
		shader->SetMat4("u_ViewProjectionMat", s_ViewProjectionMat);
		shader->SetMat4("u_ModelMat", transform);

		mesh->GetVertexArray()->Bind();

		RenderCommand::DrawIndexed(mesh->GetVertexArray());
	}

	Ref<CubeMap> Renderer::GetSkybox()
	{
		return s_SkyBox;
	}

	void Renderer::SetSkybox(Ref<CubeMap> cubemap)
	{
		s_SkyBox = cubemap;
	}

	void Renderer::AddToRenderQueue(Renderable* renderable)
	{
		s_RenderList.push_back(renderable);

		for (int i = 1; i < 100; i++)
		{
			if (s_ObjectIDList[i] == nullptr)
			{
				s_ObjectIDList[i] = renderable;
				renderable->SetID(i);

				break;
			}
		}
	}

	void Renderer::RemoveFromRenderQueue(Renderable* renderable)
	{
		s_RenderList.erase(std::remove(s_RenderList.begin(), s_RenderList.end(), renderable), s_RenderList.end());

		s_ObjectIDList[renderable->GetID()] = nullptr;
		renderable->SetID(0);
	}

	void Renderer::AddToLightList(LightEmitter* light)
	{
		s_LightsList.push_back(light);
	}

	void Renderer::RemoveFromLightList(LightEmitter* light)
	{
		s_LightsList.erase(std::remove(s_LightsList.begin(), s_LightsList.end(), light), s_LightsList.end());
	}

	const glm::vec3& Renderer::GetCameraPos()
	{
		return s_CameraPosition;
	}

	const glm::mat4& Renderer::GetViewMat()
	{
		return s_ViewMat;
	}

	const glm::mat4& Renderer::GetProjectionMat()
	{
		return s_ProjectionMat;
	}

	const glm::mat4& Renderer::GetViewProjectionMat()
	{
		return s_ViewProjectionMat;
	}


}