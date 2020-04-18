#include <Hildur.h>
#include <Hildur/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"


//class ExampleLayer : public Hildur::Layer
//{
//public:
//	ExampleLayer()
//		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
//	{
//		m_VertexArray.reset(Hildur::VertexArray::Create());
//
//		std::vector<Hildur::Vertex> vertices = {
//			{glm::vec3(-0.5f, -0.5f,  0.0f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(0.5f,  -0.5f,  0.0f),   glm::vec2(1.0f,  0.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(0.5f,   0.5f,  0.0f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(0.5f,   0.5f,  0.0f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(-0.5f,  0.5f,  0.0f),   glm::vec2(0.0f,  1.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(-0.5f, -0.5f,  0.0f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f,  0.0f, -1.0f)}
//		};
//
//		Hildur::Ref<Hildur::VertexBuffer> vertexBuffer;
//		vertexBuffer = Hildur::VertexBuffer::Create(vertices, sizeof(vertices)));
//		Hildur::BufferLayout layout = {
//			{ Hildur::ShaderDataType::Float3, "a_Position" },
//			{ Hildur::ShaderDataType::Float4, "a_Color" }
//		};
//		vertexBuffer->SetLayout(layout);
//		m_VertexArray->AddVertexBuffer(vertexBuffer);
//
//		std::vector<uint32_t> indices = { 0, 1, 2 };
//		Hildur::Ref<Hildur::IndexBuffer> indexBuffer;
//		indexBuffer = Hildur::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
//		m_VertexArray->SetIndexBuffer(indexBuffer);
//
//		m_SquareVA.reset(Hildur::VertexArray::Create());
//
//		float squareVertices[5 * 4] = {
//			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
//			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
//			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
//		};
//
//		Hildur::Ref<Hildur::VertexBuffer> squareVB;
//		squareVB = Hildur::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
//		squareVB->SetLayout({
//			{ Hildur::ShaderDataType::Float3, "a_Position" },
//			{ Hildur::ShaderDataType::Float2, "a_TexCoord" }
//			});
//		m_SquareVA->AddVertexBuffer(squareVB);
//
//		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
//		Hildur::Ref<Hildur::IndexBuffer> squareIB;
//		squareIB = Hildur::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
//		m_SquareVA->SetIndexBuffer(squareIB);
//
//		std::string vertexSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) in vec3 a_Position;
//			layout(location = 1) in vec4 a_Color;
//			uniform mat4 u_ViewProjection;
//			uniform mat4 u_Transform;
//			out vec3 v_Position;
//			out vec4 v_Color;
//			void main()
//			{
//				v_Position = a_Position;
//				v_Color = a_Color;
//				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
//			}
//		)";
//
//		std::string fragmentSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) out vec4 color;
//			in vec3 v_Position;
//			in vec4 v_Color;
//			void main()
//			{
//				color = vec4(v_Position * 0.5 + 0.5, 1.0);
//				color = v_Color;
//			}
//		)";
//
//		m_Shader = Hildur::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);
//
//		std::string flatColorShaderVertexSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) in vec3 a_Position;
//			uniform mat4 u_ViewProjection;
//			uniform mat4 u_Transform;
//			out vec3 v_Position;
//			void main()
//			{
//				v_Position = a_Position;
//				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
//			}
//		)";
//
//		std::string flatColorShaderFragmentSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) out vec4 color;
//			in vec3 v_Position;
//			
//			uniform vec3 u_Color;
//			void main()
//			{
//				color = vec4(u_Color, 1.0);
//			}
//		)";
//
//		m_FlatColorShader = Hildur::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);
//
//		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
//
//		m_Texture = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");
//		m_ChernoLogoTexture = Hildur::Texture2D::Create("assets/textures/ChernoLogo.png");
//
//		std::dynamic_pointer_cast<Hildur::OpenGLShader>(textureShader)->Bind();
//		std::dynamic_pointer_cast<Hildur::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
//	}
//
//	void OnUpdate(Hildur::Timestep ts) override
//	{
//		// Update
//		m_CameraController.OnUpdate(ts);
//
//		// Render
//		Hildur::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
//		Hildur::RenderCommand::Clear();
//
//		Hildur::Renderer::BeginScene(m_CameraController.GetCamera());
//
//		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
//
//		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_FlatColorShader)->Bind();
//		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
//
//		for (int y = 0; y < 20; y++)
//		{
//			for (int x = 0; x < 20; x++)
//			{
//				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
//				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
//				Hildur::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
//			}
//		}
//
//		auto textureShader = m_ShaderLibrary.Get("Texture");
//
//		m_Texture->Bind();
//		Hildur::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
//		m_ChernoLogoTexture->Bind();
//		Hildur::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
//
//		// Triangle
//		// Hildur::Renderer::Submit(m_Shader, m_VertexArray);
//
//		Hildur::Renderer::EndScene();
//	}
//
//	virtual void OnImGuiRender() override
//	{
//		ImGui::Begin("Settings");
//		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
//		ImGui::End();
//	}
//
//	void OnEvent(Hildur::Event& e) override
//	{
//		m_CameraController.OnEvent(e);
//
//		if (e.GetEventType() == Hildur::EventType::WindowResize) {
//
//			auto& re = (Hildur::WindowResizeEvent&)e;
//
//			float zoom = (float)re.GetWidth() / 1960;
//
//			m_CameraController.SetZoomLevel(re.GetWidth)
//
//		}
//	}
//private:
//	Hildur::ShaderLibrary m_ShaderLibrary;
//	Hildur::Ref<Hildur::Shader> m_Shader;
//	Hildur::Ref<Hildur::VertexArray> m_VertexArray;
//
//	Hildur::Ref<Hildur::Shader> m_FlatColorShader;
//	Hildur::Ref<Hildur::VertexArray> m_SquareVA;
//
//	Hildur::Ref<Hildur::Texture2D> m_Texture, m_ChernoLogoTexture;
//
//	Hildur::OrthographicCameraController m_CameraController;
//	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
//};
//
//
//class Layer2D : public Hildur::Layer {
//
//public:
//
//	Layer2D()
//		:Layer("Example"), m_CameraController(16.0f / 9.0f, true), m_TrianglePos(0.2f, 0.0f, 0.0f)
//	{
//
//		m_VertexArray.reset(Hildur::VertexArray::Create());
//
//		std::vector<Hildur::Vertex> vertices = {
//
//			//Position                        UV                         Normal
//			{glm::vec3(-0.5f, -0.5f,  0.0f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(0.5f,  -0.5f,  0.0f),   glm::vec2(1.0f,  0.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(0.5f,   0.5f,  0.0f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(0.5f,   0.5f,  0.0f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(-0.5f,  0.5f,  0.0f),   glm::vec2(0.0f,  1.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
//			{glm::vec3(-0.5f, -0.5f,  0.0f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f,  0.0f, -1.0f)}
//
//		};
//
//		std::vector<uint32_t> indices = { 0, 1, 2, 3, 4, 5 };
//
//		Hildur::Ref<Hildur::VertexBuffer> vertexBuffer;
//		vertexBuffer = Hildur::VertexBuffer::Create(vertices, sizeof(vertices));
//		Hildur::BufferLayout layout = {
//			{ Hildur::ShaderDataType::Float3, "a_Position" },
//			{ Hildur::ShaderDataType::Float4, "a_Color" }
//		};
//		vertexBuffer->SetLayout(layout);
//		m_VertexArray->AddVertexBuffer(vertexBuffer);
//
//		Hildur::Ref<Hildur::IndexBuffer> indexBuffer;
//		indexBuffer = Hildur::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
//		m_VertexArray->SetIndexBuffer(indexBuffer);
//
//
//		//quad
//
//		m_QuadVertexArray.reset(Hildur::VertexArray::Create());
//
//
//		float quadVertices[4 * 5] = {
//
//			-0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//			 0.5f, -0.5f,  1.0f,  1.0f,  0.0f,
//			 0.5f,  0.5f,  1.0f,  1.0f,  1.0f,
//			-0.5f,  0.5f,  1.0f,  0.0f,  1.0f,
//
//		};
//
//		Hildur::Ref<Hildur::VertexBuffer> quadVertexBuffer;
//		quadVertexBuffer.reset(Hildur::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));
//
//		Hildur::BufferLayout quadLayout = {
//
//			{ Hildur::ShaderDataType::Float3, "a_Pos" },
//			{ Hildur::ShaderDataType::Float2, "a_TextPos" }
//
//		};
//
//		quadVertexBuffer->SetLayout(quadLayout);
//		m_QuadVertexArray->AddVertexBuffer(quadVertexBuffer);
//
//		uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };
//		Hildur::Ref<Hildur::IndexBuffer> quadIndexBuffer;
//		quadIndexBuffer.reset(Hildur::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
//
//		m_QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
//
//		//Shader
//
//		std::string vertexSrc = R"(
//			#version 140 core
//			#extension GL_ARB_explicit_attrib_location : require
//
//			
//			layout(location = 0) in vec3 a_Position;
//			layout(location = 1) in vec4 a_Color;
//
//			uniform mat4 u_ViewProjection;
//			uniform mat4 u_Transform;
//
//			out vec3 v_Position;
//			out vec4 v_Color;
//
//			void main()
//			{
//				v_Position = a_Position;
//				v_Color = a_Color;
//				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
//			}
//		)";
//
//		std::string fragmentSrc = R"(
//			#version 140 core
//			#extension GL_ARB_explicit_attrib_location : require
//
//			
//			layout(location = 0) out vec4 color;
//
//			in vec3 v_Position;
//			in vec4 v_Color;
//
//			void main()
//			{
//				color = vec4(v_Position * 0.5 + 0.5, 1.0);
//				color = v_Color;
//			}
//		)";
//
//		m_Shader = Hildur::Shader::Create("shader", vertexSrc, fragmentSrc);
//
//
//		//Shader Texture test
//
//		std::string textureVertexSrc = R"(
//
//			#version 140 core
//			#extension GL_ARB_explicit_attrib_location : require
//
//			layout(location = 0) in vec3 a_Position;
//			layout(location = 1) in vec2 a_TexCoord;
//
//			uniform mat4 u_ViewProjection;
//			uniform mat4 u_Transform;
//
//			out vec2 v_TexCoord;
//
//			void main()
//			{
//				v_TexCoord = a_TexCoord;
//				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
//			}
//
//		)";
//
//		std::string textureFragmentSrc = R"(
//
//			#version 140 core
//			#extension GL_ARB_explicit_attrib_location : require
//
//			layout(location = 0) out vec4 color;
//			in vec2 v_TexCoord;
//			
//			uniform sampler2D u_Texture;
//			uniform sampler2D u_Texture2;
//
//			void main()
//			{
//				color = mix(texture(u_Texture, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
//				//color = texture(u_Texture2, v_TexCoord);
//				//color = vec4(0.8, 0.2, 0.3, 1.0);
//				//color = vec4(v_TexCoord, 0.0, 1.0);
//			}
//
//		)";
//
//		m_TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
//
//		m_Texture = Hildur::Texture2D::Create("assets/textures/ChernoLogo.png");
//		m_Texture2 = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");
//
//		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->Bind();
//		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 1);
//		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture2", 2);
//
//		m_Texture->Bind(1);
//		m_Texture2->Bind(2);
//
//	}
//
//	void OnUpdate(Hildur::Timestep ts) override {
//
//		//HR_TRACE("Delta Shit: {0}, ({1}ms)", ts.GetTimeSeconds(), ts.GetTimeMiliseconds());
//
//		//Update
//
//		m_CameraController.OnUpdate(ts);
//
//		//Render
//
//
//		glm::mat4 position = glm::translate(glm::mat4(1.0f), m_TrianglePos);
//		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
//		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0.0f, 0.0f, 1.0f));
//
//		glm::mat4 transform = rotation * position * scale;
//
//
//		Hildur::RenderCommand::SetClearColor({ 0.2f, 0.22f, 0.25f, 1 });
//		Hildur::RenderCommand::Clear();
//
//
//		Hildur::Renderer::BeginScene(m_CameraController.GetCamera());
//
//		// Triangle
//		Hildur::Renderer::Submit(m_Shader, m_VertexArray);
//
//		auto textureShader = m_ShaderLibrary.Get("Texture");
//		Hildur::Renderer::Submit(textureShader, m_QuadVertexArray, transform);
//
//
//		Hildur::Renderer::EndScene();
//
//	}
//
//	void OnImGuiRender() override {
//
//
//
//	}
//
//	void OnEvent(Hildur::Event& e) override {
//
//		m_CameraController.OnEvent(e);
//
//	}
//
//private:
//
//	Hildur::ShaderLibrary m_ShaderLibrary;
//
//	Hildur::Ref<Hildur::Shader> m_QuadShader, m_TextureShader;
//	Hildur::Ref<Hildur::VertexArray> m_QuadVertexArray;
//
//	Hildur::Ref<Hildur::Texture2D> m_Texture;
//	Hildur::Ref<Hildur::Texture2D> m_Texture2;
//
//	Hildur::Ref<Hildur::Shader> m_Shader;
//	Hildur::Ref<Hildur::VertexArray> m_VertexArray;
//
//
//	Hildur::OrthographicCameraController m_CameraController;
//
//	float DeltaX = 0;
//	float DeltaY = 0;
//	float rot = 0;
//
//	glm::vec3 m_TrianglePos;
//
//};



 /////// Layer 3D //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// 

/*
class Layer3D : public Hildur::Layer {

public:

	Layer3D()
		:Layer("Test3d"), m_CameraController(45.0f, 16.0f / 9.0f, true), m_ObjectRot(0.0f, 0.0f, 0.0f)
	{

		//Cube

		std::vector<Hildur::Vertex> vertices = {

			//Position                        UV                         Normal
			{glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(1.0f,  0.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(0.0f,  1.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f,  0.0f, -1.0f)},
																								  	   
			{glm::vec3(-0.5f, -0.5f,  0.5f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f,  0.0f,  1.0f)},
			{glm::vec3( 0.5f, -0.5f,  0.5f),   glm::vec2(1.0f,  0.0f),    glm::vec3(0.0f,  0.0f,  1.0f)},
			{glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f,  0.0f,  1.0f)},
			{glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f,  0.0f,  1.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(0.0f,  1.0f),    glm::vec3(0.0f,  0.0f,  1.0f)},
			{glm::vec3(-0.5f, -0.5f,  0.5f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f,  0.0f,  1.0f)},
																								  	   
			{glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(1.0f,  0.0f),   -glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(1.0f,  1.0f),   -glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(0.0f,  1.0f),   -glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(0.0f,  1.0f),   -glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f,  0.5f),   glm::vec2(0.0f,  0.0f),   -glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(1.0f,  0.0f),   -glm::vec3(1.0f,  0.0f,  0.0f)},
																								  	   
			{glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f,  0.0f),    glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f,  1.0f),    glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(0.0f,  1.0f),    glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(0.0f,  1.0f),    glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3( 0.5f, -0.5f,  0.5f),   glm::vec2(0.0f,  0.0f),    glm::vec3(1.0f,  0.0f,  0.0f)},
			{glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f,  0.0f),    glm::vec3(1.0f,  0.0f,  0.0f)},
																							  	   
			{glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(0.0f,  1.0f),    glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3( 0.5f, -0.5f,  0.5f),   glm::vec2(1.0f,  0.0f),    glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3( 0.5f, -0.5f,  0.5f),   glm::vec2(1.0f,  0.0f),    glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f,  0.5f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f, -1.0f,  0.0f)},
			{glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec2(0.0f,  1.0f),    glm::vec3(0.0f, -1.0f,  0.0f)},
																								  	   
			{glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(0.0f,  1.0f),    glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3( 0.5f,  0.5f, -0.5f),   glm::vec2(1.0f,  1.0f),    glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f,  0.0f),    glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3( 0.5f,  0.5f,  0.5f),   glm::vec2(1.0f,  0.0f),    glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3(-0.5f,  0.5f,  0.5f),   glm::vec2(0.0f,  0.0f),    glm::vec3(0.0f,  1.0f,  0.0f)},
			{glm::vec3(-0.5f,  0.5f, -0.5f),   glm::vec2(0.0f,  1.0f),    glm::vec3(0.0f,  1.0f,  0.0f)},
			
		};

		std::vector<uint32_t> indices { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };


		//Shader

		Hildur::Ref<Hildur::Shader> m_TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		Hildur::Ref<Hildur::Shader> m_LightShader = m_ShaderLibrary.Load("assets/shaders/Light.glsl");

		Hildur::Ref<Hildur::Shader> m_ColorShader = m_ShaderLibrary.Load("assets/shaders/Color.glsl");


		m_Texture = Hildur::Texture2D::Create("assets/textures/ChernoLogo.png");
		m_Texture2 = Hildur::Texture2D::Create("assets/textures/Ricardo_Milos.png");
		m_Texture->type = Hildur::HR_DIFFUSE;
		m_Texture->type = Hildur::HR_SPECULAR;

		m_Texture->Bind(1);
		m_Texture2->Bind(2);

		m_Material = Hildur::Material::Create(glm::vec3(0.0f), glm::vec3(0.4f), glm::vec3(0.1), m_Texture, m_Texture2);

		std::vector<Hildur::Ref<Hildur::Texture2D>> textures{

			m_Texture,
			m_Texture2

		};

		//m_Mesh = Hildur::Mesh::Create(vertices, indices, textures);


		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 1);
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture2", 2);

		m_CameraController.SetPosition({ 0.0f, 0.0f, -1.5f });

		m_Model = Hildur::Model::Create("assets/models/shaderball.obj");
		//m_cubeModel = Hildur::Model::Create("assets/models/cube.obj");

		//Test Scene
		//t_Scene = Hildur::Scene::Create();
		//t_Scene2 = Hildur::Scene::Create();

		//t_GameObject = Hildur::GameObject::Create("Pito de ricardo", m_Mesh);
		//t_GameObject2 = Hildur::GameObject::Create("Pito de ricardo", m_Mesh);

		//t_Scene->AddGameObject(t_GameObject);
		//t_Scene->AddGameObject(t_GameObject2, t_GameObject->GetID());
		
		//t_Scene2->AddGameObject(t_GameObject);
		//t_Scene2->AddGameObject(t_GameObject2);

		m_ObjectRot.x = 180;

		m_ObjectPos = glm::vec3(0.0f, 0.0f, 5.0f);

		ambient = 0.3f;
		diffuse = 15.0f;
		specular = 5.0f;

		LightPos = glm::vec3(-10.f, 0.0f, -10.0f);

		m_CameraController.SetPosition(glm::vec3(0.0f, 0.0f, -4.0f));


	}

	void OnUpdate(Hildur::Timestep ts) override {

		//HR_TRACE("Delta Shit: {0}, ({1}ms)", ts.GetTimeSeconds(), ts.GetTimeMiliseconds());

		//Update

		m_CameraController.OnUpdate(ts);

		m_ObjectRot.y += translationSpeed * ts * 20;

		if (Hildur::Input::IsKeyPressed(HR_KEY_R))
			m_ObjectRot.y -= translationSpeed * ts;
		else if (Hildur::Input::IsKeyPressed(HR_KEY_T))
			m_ObjectRot.y += translationSpeed * ts;

		if (Hildur::Input::IsKeyPressed(HR_KEY_H))
			LightPos.x -= ts;
		else if (Hildur::Input::IsKeyPressed(HR_KEY_K))
			LightPos.x += ts;

		if (Hildur::Input::IsKeyPressed(HR_KEY_U))
			LightPos.z += ts * 2;
		else if (Hildur::Input::IsKeyPressed(HR_KEY_J))
			LightPos.z -= ts * 2;

		if (Hildur::Input::IsKeyPressed(HR_KEY_I))
			LightPos.y -= ts * 2;
		else if (Hildur::Input::IsKeyPressed(HR_KEY_O))
			LightPos.y += ts * 2;


		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_ShaderLibrary.Get("Texture"))->UploadUniformFloat3("u_ViewPos", m_CameraController.GetCamera().GetPosition());
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_ShaderLibrary.Get("Texture"))->UploadUniformFloat3("u_LightPos0", LightPos);

		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_ShaderLibrary.Get("Color"))->UploadUniformFloat3("u_ViewPos", m_CameraController.GetCamera().GetPosition());
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_ShaderLibrary.Get("Color"))->UploadUniformFloat3("u_LightPos0", LightPos);


		glm::mat4 lightTransform = glm::translate(glm::mat4(1.0f), LightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		//Render

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_ObjectRot.x), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0F), glm::radians(m_ObjectRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

		glm::mat4 transform = rotation * scale * position;


		Hildur::RenderCommand::SetClearColor({ 0.2f, 0.22f, 0.25f, 1 });
		Hildur::RenderCommand::Clear();

		Hildur::Renderer::BeginScene(m_CameraController.GetCamera());

		//auto textureShader = m_ShaderLibrary.Get("Texture");
		//m_Material->SendToShader(textureShader);
		//Hildur::Renderer::Submit(textureShader, m_Model->GetMeshes()[0]->GetVertexArray(), transform);
		
		auto colorShader = m_ShaderLibrary.Get("Color");
		m_Material->SendToShader(colorShader);
		Hildur::Renderer::Submit(colorShader, m_Model->GetMeshes()[0]->GetVertexArray(), transform);


		auto lightShader = m_ShaderLibrary.Get("Light");
		//Hildur::Renderer::Submit(lightShader, m_cubeModel->GetMeshes()[0]->GetVertexArray(), lightTransform);

		//Hildur::Renderer::Submit(textureShader, m_Mesh->GetVertexArray(), transform);
		//Hildur::Renderer::Submit(textureShader, t_Scene->GetMesh(0)->GetVertexArray(), transform);


		//for (unsigned int i = 0; i < 10; i++)
		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, cubePositions[i]);
		//	float angle = 20.0f * i;
		//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//	Hildur::Renderer::Submit(textureShader, m_Mesh->GetVertexArray(), glm::translate(model, {2.0f, 0.0f, 0.0f}));
		//}


		Hildur::Renderer::EndScene();

	}

	void OnImGuiRender() override {

		Hildur::Ref<Hildur::Shader> m_TextureShader;
		Hildur::Ref<Hildur::Shader> m_LightShader;

		ImGui::Begin("Debug settings");

		if (ImGui::Button("Reload Main Shader")) {

			m_TextureShader = m_ShaderLibrary.Reload("assets/shaders/Texture.glsl");
			m_LightShader = m_ShaderLibrary.Reload("assets/shaders/Light.glsl");

		}

		ImGui::SliderFloat("Ambient", &ambient, 0.0f, 50.0f);
		ImGui::SliderFloat("Diffuse", &diffuse, 0.0f, 50.0f);
		ImGui::SliderFloat("Specular", &specular, 0.0f, 50.0f);

		ImGui::SliderFloat3("Light Color", (&LightColor.r, &LightColor.g, &LightColor.b), 0.0f, 1.0f);
		ImGui::ColorEdit3("Light Color", (&LightColor.r, &LightColor.g, &LightColor.b));

		ImGui::SliderFloat3("Light Position", (&LightPos.x, &LightPos.y, &LightPos.z), -100.0f, 100.0f);

		ImGui::SliderFloat("RotationSpeed", &translationSpeed, 0.0f, 50.0f);



		ImGui::End();

		m_Material = Hildur::Material::Create(glm::vec3(ambient), LightColor * glm::vec3(diffuse), LightColor * glm::vec3(specular), m_Texture, m_Texture2);

	}

	void OnEvent(Hildur::Event& e) override {

		m_CameraController.OnEvent(e);

	}

private:

	Hildur::ShaderLibrary m_ShaderLibrary;

	Hildur::Ref<Hildur::Texture2D> m_Texture;
	Hildur::Ref<Hildur::Texture2D> m_Texture2;

	Hildur::Ref<Hildur::Material> m_Material;

	Hildur::Ref<Hildur::Mesh> m_Mesh;
	

	Hildur::PerspectiveCameraController m_CameraController;


	glm::vec3 m_ObjectRot;
	glm::vec3 m_ObjectPos;

	float translationSpeed = 1;

	glm::vec3 cubePositions[10] = {

		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)

	};

	glm::vec3 LightPos = glm::vec3(0.0f);

	float ambient = 0;
	float diffuse = 0;
	float specular = 0;
	glm::vec3 LightColor = glm::vec3(1.0f);

	//Model
	Hildur::Ref<Hildur::Model> m_Model;
	Hildur::Ref<Hildur::Model> m_cubeModel;

	//Test scene

};
*/


class ImGuiLayer : public Hildur::ImGuiLayer
{
public:

	ImGuiLayer() 
	{
	}

	~ImGuiLayer() 
	{
	}
};

class ImaginaryScene : public Hildur::Scene 
{
public:

	void Load() 
	{
		Hildur::Entity* mike = Scene::instantiate("manuel");
		mike->getComponent<Hildur::Transform>()->setPosition(glm::vec3(8.0f, 3.0f, 6.0f));

		glm::vec3 pos = mike->getComponent<Hildur::Transform>()->getPosition();

		HR_CORE_TRACE("Mike's position is: {0}, {1}, {2}", pos.x, pos.y, pos.z);
	}
};

class SuperScene : public Hildur::Scene 
{
public:

	void Load()
	{
		Hildur::Entity* manuel = Scene::instantiate("manuel");
		manuel->getComponent<Hildur::Transform>()->setPosition(glm::vec3(1.0f, 12.0f, 5.0f));

		glm::vec3 pos = manuel->getComponent<Hildur::Transform>()->getPosition();

		HR_CORE_TRACE("Manuel's position is: {0}, {1}, {2}", pos.x, pos.y, pos.z);
	}
};

class MegaScene : public Hildur::Scene 
{
public:

	void Load()
	{
		Hildur::Entity* peter = Scene::instantiate("manuel");
		peter->getComponent<Hildur::Transform>()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		glm::vec3 pos = peter->getComponent<Hildur::Transform>()->getPosition();

		HR_CORE_TRACE("Peter's position is: {0}, {1}, {2}", pos.x, pos.y, pos.z);
	}
};


class Sandbox : public Hildur::Application 
{
public:

	Sandbox() 
	{
		//PushLayer(new Layer2D());
		//PushLayer(new Layer3D());

		//std::map<std::string, Hildur::Scene*> scenes;
		//scenes["ImaginaryScene"] = new ImaginaryScene();
		//scenes["SuperScene"] = new SuperScene();
		//scenes["MegaScene"] = new MegaScene();

		//Init(scenes);

		PushLayer(new Sandbox2D());
	}

	~Sandbox() 
	{
	}
};

Hildur::Application* Hildur::CreateApplication()
{
	return new Sandbox;
}