#include "hrpcheaders.h"
#include "OpenGLBuffer.h"

#include "Hildur/Renderer/RenderCommand.h"

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

	//  Frame Buffer  ////////////////////////////////////////////////////////

	OpenGLFrameBuffer::OpenGLFrameBuffer(const uint32_t width, const uint32_t height, uint32_t texture) {

		// Create a framebuffer
		glGenFramebuffers(1, &m_RendererID);
		RenderCommand::SetRenderTarget(m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		// Create a texure if not provided
		if (texture == 0) {

			glGenTextures(1, &texture);

		}

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //TODO: dynamically create a global Resolution variable  

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

			HR_CORE_ASSERT(false, "Could not create FrameBuffer object");

		}
		else {

			HR_CORE_INFO("FrameBuffer object created");

		}

		m_Texture = texture;

	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {



	}

	void OpenGLFrameBuffer::Bind() const {

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	}

	void OpenGLFrameBuffer::UnBind() const {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}


}