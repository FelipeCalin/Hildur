#include <Hildur.h>

#include <GLM/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <ImGui/imgui.h>
#include <GLFW/include/GLFW/glfw3.h>

//#include <Glad/include/glad/glad.h>
#include <stb_image/stb_image.h>



class Layer2D : public Hildur::Layer {

public:

	Layer2D()
		:Layer("Example"), m_CameraController(16.0f / 9.0f, true), m_TrianglePos(0.2f, 0.0f, 0.0f)
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

		m_Shader = Hildur::Shader::Create("shader", vertexSrc, fragmentSrc);


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

		m_TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Hildur::Texture2D::Create("assets/textures/ChernoLogo.png");
		m_Texture2 = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 1);
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture2", 2);

		m_Texture->Bind(1);
		m_Texture2->Bind(2);

	}

	void OnUpdate(Hildur::Timestep ts) override {

		//HR_TRACE("Delta Shit: {0}, ({1}ms)", ts.GetTimeSeconds(), ts.GetTimeMiliseconds());

		//Update

		m_CameraController.OnUpdate(ts);

		//Render


		glm::mat4 position = glm::translate(glm::mat4(1.0f), m_TrianglePos);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 transform = rotation * position * scale;


		Hildur::RenderCommand::SetClearColor({ 0.2f, 0.22f, 0.25f, 1 });
		Hildur::RenderCommand::Clear();


		Hildur::Renderer::BeginScene(m_CameraController.GetCamera());

		// Triangle
		Hildur::Renderer::Submit(m_Shader, m_VertexArray);

		auto textureShader = m_ShaderLibrary.Get("Texture");
		Hildur::Renderer::Submit(textureShader, m_QuadVertexArray, transform);


		Hildur::Renderer::EndScene();

	}

	void OnImGuiRender() override {



	}

	void OnEvent(Hildur::Event& e) override {

		m_CameraController.OnEvent(e);

	}

private:

	Hildur::ShaderLibrary m_ShaderLibrary;

	Hildur::Ref<Hildur::Shader> m_QuadShader, m_TextureShader;
	Hildur::Ref<Hildur::VertexArray> m_QuadVertexArray;

	Hildur::Ref<Hildur::Texture2D> m_Texture;
	Hildur::Ref<Hildur::Texture2D> m_Texture2;

	Hildur::Ref<Hildur::Shader> m_Shader;
	Hildur::Ref<Hildur::VertexArray> m_VertexArray;


	Hildur::OrthographicCameraController m_CameraController;

	float DeltaX = 0;
	float DeltaY = 0;
	float rot = 0;

	glm::vec3 m_TrianglePos;

};


 /////// Layer 3D //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// 


class Layer3D : public Hildur::Layer {

public:

	Layer3D()
		:Layer("Test3d"), m_CameraController(45.0f, 16.0f / 9.0f, true), m_ObjectPos(0.2f, 0.0f, 0.0f), m_ObjectRot(0.0f, 0.0f, 0.0f)
	{

		//Cube

		m_QuadVertexArray.reset(Hildur::VertexArray::Create());

		float quadVertices[8 * 6 * 6] = {

			//Position             //UVs          //Normals
			-0.5f, -0.5f, -0.5f,   0.0f,  0.0f,   0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,   0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,   1.0f,  1.0f,   0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,   1.0f,  1.0f,   0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,   0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,   0.0f,  0.0f,   0.0f,  0.0f,  1.0f,
								  		 	    	   
			-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,   0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,   0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f,  0.5f,   1.0f,  1.0f,   0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f,  0.5f,   1.0f,  1.0f,   0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,   0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,   0.0f,  0.0f, -1.0f,
	  							  		 	    	   
			-0.5f,  0.5f,  0.5f,   1.0f,  0.0f,   1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,   1.0f,  1.0f,   1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f,  1.0f,   1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f,  1.0f,   1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,   1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,   1.0f,  0.0f,   1.0f,  0.0f,  0.0f,
 								  		 	    	   
			 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,   1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,   0.0f,  1.0f,  -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,   0.0f,  1.0f,  -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  -1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  -1.0f,  0.0f,  0.0f,
								  		 	    	   
			-0.5f, -0.5f, -0.5f,   0.0f,  1.0f,   0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,   1.0f,  1.0f,   0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,   0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,   0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,   0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f,  1.0f,   0.0f,  1.0f,  0.0f,
								  		 	    	   
			-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,   0.0f, -1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,   1.0f,  1.0f,   0.0f, -1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,   0.0f, -1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,   0.0f, -1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,   0.0f, -1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,   0.0f, -1.0f,  0.0f,

		};


		Hildur::Ref<Hildur::VertexBuffer> quadVertexBuffer;
		quadVertexBuffer.reset(Hildur::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));

		Hildur::BufferLayout quadLayout = {

			{ Hildur::ShaderDataType::Float3, "a_Pos" },
			{ Hildur::ShaderDataType::Float2, "a_TextPos" },
			{ Hildur::ShaderDataType::Float3, "a_Normal" }

		};

		quadVertexBuffer->SetLayout(quadLayout);
		m_QuadVertexArray->AddVertexBuffer(quadVertexBuffer);

		uint32_t quadIndices[36] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
		Hildur::Ref<Hildur::IndexBuffer> quadIndexBuffer;
		quadIndexBuffer.reset(Hildur::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));

		m_QuadVertexArray->SetIndexBuffer(quadIndexBuffer);


		//Shader

		m_TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		m_LightShader = m_ShaderLibrary.Load("assets/shaders/Light.glsl");

		m_Texture = Hildur::Texture2D::Create("assets/textures/ChernoLogo.png");
		m_Texture2 = Hildur::Texture2D::Create("assets/textures/Ricardo_Milos.png");

		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 1);
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture2", 2);


		m_Texture->Bind(1);
		m_Texture2->Bind(2);

		m_CameraController.SetPosition({ 0.0f, 0.0f, -1.5f });

	}

	void OnUpdate(Hildur::Timestep ts) override {

		//HR_TRACE("Delta Shit: {0}, ({1}ms)", ts.GetTimeSeconds(), ts.GetTimeMiliseconds());

		//Update

		m_CameraController.OnUpdate(ts);


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


		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformFloat3("u_ViewPos", m_CameraController.GetCamera().GetPosition());
		std::dynamic_pointer_cast<Hildur::OpenGLShader>(m_TextureShader)->UploadUniformFloat3("u_LightPos0", LightPos);


		//m_ObjectRot.y += 1.0f * ts;

		glm::mat4 lightTransform = glm::translate(glm::mat4(1.0f), LightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		//Render

		glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_ObjectRot.y, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 transform = rotation * scale * position;
		//glm::mat4 transform = rotation;


		Hildur::RenderCommand::SetClearColor({ 0.2f, 0.22f, 0.25f, 1 });
		Hildur::RenderCommand::Clear();


		Hildur::Renderer::BeginScene(m_CameraController.GetCamera());

		auto textureShader = m_ShaderLibrary.Get("Texture");
		Hildur::Renderer::Submit(textureShader, m_QuadVertexArray, transform);

		auto lightShader = m_ShaderLibrary.Get("Light");
		Hildur::Renderer::Submit(lightShader, m_QuadVertexArray, lightTransform);


		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			Hildur::Renderer::Submit(textureShader, m_QuadVertexArray, glm::translate(model, {2.0f, 0.0f, 0.0f}));
		}


		Hildur::Renderer::EndScene();

	}

	void OnImGuiRender() override {

		ImGui::Begin("Debug settings");

		if (ImGui::Button("Reload Main Shader")) {

			m_TextureShader = m_ShaderLibrary.Reload("assets/shaders/Texture.glsl");
			m_LightShader = m_ShaderLibrary.Reload("assets/shaders/Light.glsl");

		}

		ImGui::End();

	}

	void OnEvent(Hildur::Event& e) override {

		m_CameraController.OnEvent(e);

	}

private:

	Hildur::ShaderLibrary m_ShaderLibrary;

	Hildur::Ref<Hildur::Shader> m_QuadShader, m_TextureShader, m_LightShader;
	Hildur::Ref<Hildur::VertexArray> m_QuadVertexArray;

	Hildur::Ref<Hildur::Texture2D> m_Texture;
	Hildur::Ref<Hildur::Texture2D> m_Texture2;

	Hildur::Ref<Hildur::Shader> m_Shader;
	Hildur::Ref<Hildur::VertexArray> m_VertexArray;


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

		//PushLayer(new Layer2D());
		PushLayer(new Layer3D());

	}

	~Sandbox() {



	}

};

Hildur::Application* Hildur::CreateApplication() {

	return new Sandbox;

}