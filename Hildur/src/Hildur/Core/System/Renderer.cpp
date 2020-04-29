#include "hrpcheaders.h"
#include "Renderer.h"

#include "Hildur/Resource/Texture.h"
#include "Hildur/Component/Renderable.h" 

#include "Hildur/Renderer/RenderCommand.h"
#include "Hildur/Core/System/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>


namespace Hildur {


	struct RendererStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		std::vector<Renderable*> RenderList;
		std::vector<LightEmitter*> LightsList;
	};

	static RendererStorage* s_Data;
	static glm::mat4 s_ViewProjectionMat;

	void Renderer::Init() 
	{
		HR_PROFILE_FUNCTION();
		
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		s_ViewProjectionMat = camera.GetViewProjectionMatrix();

		//TODO: Only update uniforms on active scene materials
	}

	void Renderer::BeginScene(PerspectiveCamera& camera) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		s_ViewProjectionMat = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() 
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		shader->Bind();
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


	void Renderer::AddToRenderQueue(Renderable* renderable)
	{
		s_Data->RenderList.push_back(renderable);
	}

	void Renderer::RemoveFromRenderQueue(Renderable* renderable)
	{
		s_Data->RenderList.erase(std::remove(s_Data->RenderList.begin(), s_Data->RenderList.end(), renderable), s_Data->RenderList.end());
	}

	void Renderer::AddToLightList(LightEmitter* light)
	{
		s_Data->LightsList.push_back(light);
	}

	void Renderer::RemoveFromLightList(LightEmitter* light)
	{
		s_Data->LightsList.erase(std::remove(s_Data->LightsList.begin(), s_Data->LightsList.end(), light), s_Data->LightsList.end());
	}


}