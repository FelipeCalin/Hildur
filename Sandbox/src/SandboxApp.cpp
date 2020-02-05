#include <Hildur.h>

#include <GLM/gtc/matrix_transform.hpp>


class ExampleLayer : public Hildur::Layer {

public:

	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_TrianglePos(0.2f, 0.0f, 0.0f)
	{

		m_VertexArray.reset(Hildur::VertexArray::Create());


		float vertices[3 * 7] = {

			-0.5f, -0.5f, 1.0f, 0.8f, 0.0f, 0.2f, 1.0f,
			 0.5f, -0.5f, 1.0f, 0.0f, 0.8f, 0.2f, 1.0f,
			 0.0f,  0.5f, 1.0f, 0.2f, 0.8f, 1.0f, 1.0f

		};

		std::shared_ptr<Hildur::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hildur::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hildur::BufferLayout layout = {

			{ Hildur::ShaderDataType::Float3, "a_Pos" },
			{ Hildur::ShaderDataType::Float4, "a_Color" }

		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hildur::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hildur::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		//Square test

		m_SquareVertexArray.reset(Hildur::VertexArray::Create());

		float squareVertices[4 * 4] = {

			-0.75f,  0.75f, 0.0f,
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,

		};

		std::shared_ptr<Hildur::VertexBuffer> SquareVertexBuffer;
		SquareVertexBuffer.reset(Hildur::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Hildur::BufferLayout squareVertexBufferLayout = {

			{ Hildur::ShaderDataType::Float3, "a_Pos" }

		};

		SquareVertexBuffer->SetLayout(squareVertexBufferLayout);
		m_SquareVertexArray->AddVertexBuffer(SquareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hildur::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Hildur::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);


		//Shader test 1

		std::string vertexSrc = R"(

			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec4 a_Color;

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

		std::string fragmentSrc = R"(

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

		m_Shader.reset(new Hildur::Shader(vertexSrc, fragmentSrc));


		//Shader test 2

		std::string blueShadervertexSrc = R"(

			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			layout(location = 0) in vec3 a_Pos;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Pos;

			void main() {

				v_Pos = a_Pos;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);

			}

		)";

		std::string blueShaderfragmentSrc = R"(

			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			layout(location = 0) out vec4 color;

			in vec3 v_Pos;

			void main() {

				color = vec4(0.2f, 0.8f, 1.0f, 1.0);

			}

		)";

		m_BlueShader.reset(new Hildur::Shader(blueShadervertexSrc, blueShaderfragmentSrc));


		LastX = Hildur::Input::GetMouseX();
		LastY = Hildur::Input::GetMouseY();

		DeltaX = 0;
		DeltaY = 0;

	}

	void OnUpdate(Hildur::Timestep ts) override {

		HR_TRACE("Delta Shit: {0}, ({1}ms)", ts.GetTimeSeconds(), ts.GetTimeMiliseconds());

		Hildur::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Hildur::RenderCommand::Clear();

		Hildur::Renderer::BeginScene(m_Camera);

		glm::mat4 position = glm::translate(glm::mat4(1.0f), m_TrianglePos);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 transform = rotation * position * scale;

		Hildur::Renderer::Submit(m_BlueShader, m_SquareVertexArray);
		Hildur::Renderer::Submit(m_Shader, m_VertexArray, transform);

		Hildur::Renderer::EndScene();

		
		/*DeltaX += (Hildur::Input::GetMouseX() - LastX) * 0.003;
		LastX = Hildur::Input::GetMouseX();

		DeltaY += (Hildur::Input::GetMouseY() - LastY) * -0.003;
		LastY = Hildur::Input::GetMouseY();*/

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

	void OnEvent(Hildur::Event& e) override {

	

	}

private:

	std::shared_ptr<Hildur::Shader> m_Shader;
	std::shared_ptr<Hildur::VertexArray> m_VertexArray;

	std::shared_ptr<Hildur::Shader> m_BlueShader;
	std::shared_ptr<Hildur::VertexArray> m_SquareVertexArray;

	std::shared_ptr<Hildur::VertexArray> m_CubeVertexArray;

	Hildur::OrthographicCamera m_Camera;

	float CameraXPos = 0;
	float CameraRot = 0;
	float LastX, LastY = 0;
	float DeltaX, DeltaY = 0;

	float rot = 0;
	glm::vec3 sc = glm::vec3(0.2f, 0.2f, 0.2f);

	glm::vec3 m_TrianglePos;

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