#include "hrpcheaders.h"
#include "Renderer.h"

#include "Hildur/Resource/Texture.h"

#include "Hildur/Renderer/RenderCommand.h"
#include "Hildur/Core/System/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Hildur {


	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();


	void Renderer::Init() {

		RenderCommand::Init();
		Renderer2D::Init();

	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {

		RenderCommand::SetViewport(0, 0, width, height);

	}

	void Renderer::BeginScene(OrthographicCamera& camera) {

		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

	}

	void Renderer::BeginScene(PerspectiveCamera& camera) {

		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

	}

	void Renderer::EndScene() {



	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformMat4("u_ViewProjectionMat", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformMat4("u_ModelMat", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform) {

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformMat4("u_ViewProjectionMat", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformMat4("u_ModelMat", transform);


		mesh->GetDiffuseTex()->Bind(mesh->GetDiffuseTex()->GetTextureUnit());
		std::dynamic_pointer_cast<OpenGLShader> (shader)->UploadUniformInt("diffuseTex1", mesh->GetDiffuseTex()->GetTextureUnit());


		mesh->GetVertexArray()->Bind();

		RenderCommand::DrawIndexed(mesh->GetVertexArray());

	}

}