#include "hrpcheaders.h"
#include "Application.h"

#include "Core.h"
#include "Input.h"

#include <glad/glad.h>
#include <imgui.h>

namespace Hildur {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Hildur::ShaderDataType::Float:    return GL_FLOAT;
		case Hildur::ShaderDataType::Float2:   return GL_FLOAT;
		case Hildur::ShaderDataType::Float3:   return GL_FLOAT;
		case Hildur::ShaderDataType::Float4:   return GL_FLOAT;
		case Hildur::ShaderDataType::Mat3:     return GL_FLOAT;
		case Hildur::ShaderDataType::Mat4:     return GL_FLOAT;
		case Hildur::ShaderDataType::Int:      return GL_INT;
		case Hildur::ShaderDataType::Int2:     return GL_INT;
		case Hildur::ShaderDataType::Int3:     return GL_INT;
		case Hildur::ShaderDataType::Int4:     return GL_INT;
		case Hildur::ShaderDataType::Bool:     return GL_BOOL;
		}

		HR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


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

		

		float vertices[3 * 7] = {

			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f

		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));


		BufferLayout layout = {

			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float4, "a_Color" }

		};

		m_VertexBuffer->SetLayout(layout);

		uint32_t index = 0;
		for (const auto& element : m_VertexBuffer->GetLayout()) {

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(), 
				(const void*) element.Offset);
			index++;
		}

		//m_VertexBuffer->SetLayout(layout);
		
		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		//TEST

		std::string vertexSrc = R"(

			#version 140 core
			#extension GL_ARB_explicit_attrib_location : require

			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Pos;
			out vec4 v_Col;

			void main() {

			v_Pos = a_Pos;
			v_Col = a_Color;
			gl_Position = vec4(a_Pos, 1.0);

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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));


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

			m_Shader->Bind();

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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