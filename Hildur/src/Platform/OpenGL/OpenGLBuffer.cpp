#include "hrpcheaders.h"
#include "OpenGLBuffer.h"

#include "Hildur/Renderer/RenderCommand.h"

#include <glad/glad.h>


namespace Hildur {


	//  Vertex Buffer  ///////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t count)
	{
		HR_PROFILE_FUNCTION()

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

		glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(Ref<std::vector<Vertex>> vertices, uint32_t count) 
	{
		HR_PROFILE_FUNCTION()

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

		glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), vertices->data(), GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() 
	{
		HR_PROFILE_FUNCTION()

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const 
	{
		HR_PROFILE_FUNCTION()

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::UnBind() const	
	{
		HR_PROFILE_FUNCTION()

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	//  Index Buffer  ////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count) 
	{
		HR_PROFILE_FUNCTION()

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(Ref<std::vector<uint32_t>> indices, uint32_t count)
		: m_Count(count) 
	{
		HR_PROFILE_FUNCTION()

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices->data(), GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() 
	{
		HR_PROFILE_FUNCTION()

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const 
	{
		HR_PROFILE_FUNCTION()

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::UnBind() const 
	{
		HR_PROFILE_FUNCTION()

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


}