#include "hrpcheaders.h"
#include "Application.h"

#include "Core.h"

#include "Input.h"

#include <glad/glad.h>
#include <imgui.h>

namespace Hildur {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	Application::Application() {

		HR_CORE_ASSERT(!s_Instance, "Application already exists")
		s_Instance = this;

		//m_Window = Window::Create();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		//m_ImGuiLayer = std::make_unique<ImGuiLayer>();
		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		//Test

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3] = {

			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f

		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		//Vertex Shader

		const char* vertexShaderSource = "#version 140 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";
		

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		//HR_CORE_ASSERT(!success, "Shader failed to compile")
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			HR_ERROR("Error in compilation of shader: {0}", infoLog);
		}


		//Fragment Shader

		const char* fragmentShaderSource = "#version 140 core\n"
			"out vec4 FragColor\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\0";

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);


		//Shader Program

		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			HR_ERROR("Error linking shader program: {0}", infoLog);
		}

		glUseProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//End

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//TEST

		glClearColor(0.2f, 0.2f, 0.2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

	}

	Application::~Application() {



	}

	void Application::OnEvent(Event& e) {

		EventDispatcher distpatcher(e);
		distpatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindoClose));

		//HR_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {

			(*--it)->OnEvent(e);
			if (e.Handled)
				break;

		}

	}

	void Application::Run() {

		while (m_Running) {

			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* ImGuiLayer : m_LayerStack)
				ImGuiLayer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

		}

	}

	bool Application::OnWindoClose(WindowCloseEvent& e) {

		m_Running = false;

		return true;

	}

	//Layer stack handling
	void Application::PushLayer(Layer* layer) {

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();

	}

	void Application::PushOverlay(Layer* layer) {

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

}