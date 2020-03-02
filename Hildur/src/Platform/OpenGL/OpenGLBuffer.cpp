#include "hrpcheaders.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>


namespace Hildur {


	//  Vertex Buffer  ///////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(Ref<std::vector<Vertex>> vertices, uint32_t size) {

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

		glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), vertices->data(), GL_STATIC_DRAW);

	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {

		glDeleteBuffers(1, &m_RendererID);

	}

	void OpenGLVertexBuffer::Bind() const {

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

	}

	void OpenGLVertexBuffer::UnBind() const	{

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}


	//  Index Buffer  ////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(Ref<std::vector<uint32_t>> indices, uint32_t size)
		: m_Count(size) {

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), indices->data(), GL_STATIC_DRAW);

	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {

		glDeleteBuffers(1, &m_RendererID);

	}

	void OpenGLIndexBuffer::Bind() const {

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

	}

	void OpenGLIndexBuffer::UnBind() const {

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

}