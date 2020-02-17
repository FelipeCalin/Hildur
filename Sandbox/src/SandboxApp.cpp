#include <Hildur.h>

#include <GLM/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <ImGui/imgui.h>
#include <GLFW/include/GLFW/glfw3.h>

//#include <Glad/include/glad/glad.h>
#include <stb_image/stb_image.h>


class ExampleLayer : public Hildur::Layer {

public:

	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_TrianglePos(0.2f, 0.0f, 0.0f)
	{

		m_VertexArray.reset(Hildur::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Hildur::Ref<Hildur::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hildur::VertexBuffer::Create(vertices, sizeof(vertices)));
		Hildur::BufferLayout layout = {
			{ Hildur::ShaderDataType::Float3, "a_Position" },
			{ Hildur::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Hildur::Ref<Hildur::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hildur::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		//quad

		m_QuadVertexArray.reset(Hildur::VertexArray::Create());


		float quadVertices[4 * 5] = {

			-0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  1.0f,  0.0f,  1.0f,

		};

		Hildur::Ref<Hildur::VertexBuffer> quadVertexBuffer;
		quadVertexBuffer.reset(Hildur::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));

		Hildur::BufferLayout quadLayout = {

			{ Hildur::ShaderDataType::Float3, "a_Pos" },
			{ Hildur::ShaderDataType::Float2, "a_TextPos" }

		};

		quadVertexBuffer->SetLayout(quadLayout);
		m_QuadVertexArray->AddVertexBuffer(quadVertexBuffer);

		uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Hildur::Ref<Hildur::IndexBuffer> quadIndexBuffer;
		quadIndexBuffer.reset(Hildur::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));

		m_QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

		//Shader

		std::string vertexSrc = R"(
			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(Hildur::Shader::Create(vertexSrc, fragmentSrc));

		//Shader test 

		std::string quadVertexSrc = R"(

			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			layout(location = 0) in vec3 a_Pos;
			layout(location = 2) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Pos;
			out vec4 v_Col;

			void main() {

			v_Pos = a_Pos;
			v_Col = a_Color;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);

			}

		)";

		std::string quadFragmentSrc = R"(

			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			layout(location = 0) out vec4 color;

			in vec3 v_Pos;
			in vec4 v_Col;

			void main() {

			color = vec4(v_Col);
			//color = vec4(v_Pos * 0.5 + 0.5, 1.0);

			}

		)";

		m_QuadShader.reset(Hildur::Shader::Create(quadVertexSrc, quadFragmentSrc));

		//Shader Texture test

		std::string textureVertexSrc = R"(

			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}

		)";

		std::string textureFragmentSrc = R"(

			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;
			uniform sampler2D u_Texture2;

			void main()
			{
				color = mix(texture(u_Texture, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
				//color = texture(u_Texture2, v_TexCoord);
				//color = vec4(0.8, 0.2, 0.3, 1.0);
				//color = vec4(v_TexCoord, 0.0, 1.0);
			}

		)";

		m_TextureShader.reset(Hildur::Shader::Create("assets/shaders/Texture.glsl"));


		m_Texture = Hildur::Texture2D::Create("assets/textures/flo.png");
		m_Texture2 = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 1);
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture2", 2);

		m_Texture->Bind(1);
		m_Texture2->Bind(2);

	}

	void OnUpdate(Hildur::Timestep ts) override {

		//HR_TRACE("Delta Shit: {0}, ({1}ms)", ts.GetTimeSeconds(), ts.GetTimeMiliseconds());

		glm::mat4 position = glm::translate(glm::mat4(1.0f), m_TrianglePos);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 transform = rotation * position * scale;


		Hildur::RenderCommand::SetClearColor({ 0.2f, 0.22f, 0.25f, 1 });
		Hildur::RenderCommand::Clear();


		Hildur::Renderer::BeginScene(m_Camera);

		// Triangle
		Hildur::Renderer::Submit(m_Shader, m_VertexArray);

		Hildur::Renderer::Submit(m_TextureShader, m_QuadVertexArray, transform);


		Hildur::Renderer::EndScene();


		if (Hildur::Input::IsKeyPressed(HR_KEY_UP))
			DeltaY += 0.05f;
		if (Hildur::Input::IsKeyPressed(HR_KEY_DOWN))
			DeltaY -= 0.05f;
		if (Hildur::Input::IsKeyPressed(HR_KEY_LEFT))
			DeltaX -= 0.05f;
		if (Hildur::Input::IsKeyPressed(HR_KEY_RIGHT))
			DeltaX += 0.05f;

		//CameraXPos += 0.001F;
		m_Camera.SetPosition({ DeltaX, DeltaY, 0.0f });

	}

	void OnImGuiRender() override {



	}

	void OnEvent(Hildur::Event& e) override {



	}

private:

	Hildur::Ref<Hildur::Shader> m_QuadShader, m_TextureShader;
	Hildur::Ref<Hildur::VertexArray> m_QuadVertexArray;

	Hildur::Ref<Hildur::Texture2D> m_Texture;
	Hildur::Ref<Hildur::Texture2D> m_Texture2;

	Hildur::Ref<Hildur::Shader> m_Shader;
	Hildur::Ref<Hildur::VertexArray> m_VertexArray;


	Hildur::OrthographicCamera m_Camera;

	float DeltaX = 0;
	float DeltaY = 0;
	float rot = 0;

	glm::vec3 m_TrianglePos;

};

class ImGuiLayer : public Hildur::ImGuiLayer {

public:

	ImGuiLayer() {



	}

	~ImGuiLayer() {



	}

};


class Sandbox : public Hildur::Application {

public:

	Sandbox() {

		PushLayer(new ExampleLayer());

	}

	~Sandbox() {



	}

};

Hildur::Application* Hildur::CreateApplication() {

	return new Sandbox;

}