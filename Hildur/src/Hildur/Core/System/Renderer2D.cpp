#include "hrpcheaders.h"
#include "Renderer2D.h"

#include "Hildur/Renderer/RenderCommand.h"

#include "Hildur/Renderer/VertexArray.h"
#include "Hildur/Resource/Shader.h"

#include "Hildur/Core/Component.h"
#include "Hildur/Component/Transform.h"
#include "Hildur/Component/Camera.h"
#include "Hildur/Component/Sprite.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>		// TODO: Remove dependencie, and abstract line drawing


namespace Hildur {


	static inline double GET_ABS(double x) { return x > 0 ? x : -x; }

	struct Renderer2DStorage 
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexArray> LineVertexArray;
		Ref<Shader> TextureShader;
		Ref<Shader> LineShader;
		Ref<Texture2D> WhiteTexture;

		std::vector<Sprite*> RenderList;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() 
	{
		HR_PROFILE_FUNCTION()

		s_Data = new Renderer2DStorage();

		// Quad Setup
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = 
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);


		// Line Setup
		s_Data->LineVertexArray = VertexArray::Create();

		float lineVertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> lineVB;
		lineVB = VertexBuffer::Create(lineVertices, sizeof(lineVertices));
		lineVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});

		s_Data->LineVertexArray->AddVertexBuffer(squareVB);
		s_Data->LineVertexArray->SetIndexBuffer(squareIB);


		// White Texture
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->LineShader = Shader::Create("assets/shaders/Line.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown() 
	{
		HR_PROFILE_FUNCTION()

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) 
	{
		HR_PROFILE_FUNCTION()

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjectionMat", camera.GetViewProjectionMatrix());

		s_Data->LineShader->Bind();
		s_Data->LineShader->SetMat4("u_ViewProjectionMat", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		HR_PROFILE_FUNCTION()

	}

	void Renderer2D::Prep()
	{
		s_Data->TextureShader->Bind();
		if (Camera::GetMainCamera())
			s_Data->TextureShader->SetMat4("u_ViewProjectionMat", Camera::GetMainCamera()->GetViewProjection());

		s_Data->LineShader->Bind();
		if (Camera::GetMainCamera())
			s_Data->LineShader->SetMat4("u_ViewProjectionMat", Camera::GetMainCamera()->GetViewProjection());
	}

	void Renderer2D::RenderQueue()
	{
		for (Sprite* sprite : s_Data->RenderList)
		{
			if (sprite->GetEnable())
			{
				/*s_Data->TextureShader->SetFloat4("u_Color", sprite->GetColor());
				s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
				if (sprite->GetTexture())
					sprite->GetTexture()->Bind();
				else
					s_Data->WhiteTexture->Bind();

				glm::mat4 transform = glm::translate(glm::mat4(1.0f), sprite->GetTransform()->GetPosition())
					* glm::scale(glm::mat4(1.0f), sprite->GetTransform()->GetScale());
				s_Data->TextureShader->SetMat4("u_ModelMat", transform);

				s_Data->QuadVertexArray->Bind();
				RenderCommand::DrawIndexed(s_Data->QuadVertexArray);*/
			
				sprite->Render();

			}
		}
	}

	//Primitives
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) 
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) 
	{
		HR_PROFILE_FUNCTION()

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_ModelMat", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& colorShift)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, colorShift);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& colorShift)
	{
		HR_PROFILE_FUNCTION()

		s_Data->TextureShader->SetFloat4("u_Color", colorShift);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_ModelMat", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		HR_PROFILE_FUNCTION()

		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_ModelMat", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& colorShift)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, colorShift);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& colorShift)
	{
		HR_PROFILE_FUNCTION()

		s_Data->TextureShader->SetFloat4("u_Color", colorShift);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_ModelMat", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawLine(const glm::vec2& point1, const glm::vec2& point2, const glm::vec4& color, float thickness)
	{
		HR_PROFILE_FUNCTION()

		s_Data->LineShader->SetFloat4("u_Color", color);
		s_Data->LineShader->SetMat4("u_ModelMat", glm::mat4(1.0f));

		s_Data->LineVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->LineVertexArray);
	}

	void Renderer2D::AddToRenderQueue(Sprite* spriteComponent)
	{
		s_Data->RenderList.push_back(spriteComponent);
	}

	void Renderer2D::RemoveFromRenderQueue(Sprite* spriteComponent)
	{
		s_Data->RenderList.erase(std::remove(s_Data->RenderList.begin(), s_Data->RenderList.end(), spriteComponent), s_Data->RenderList.end());
	}


}