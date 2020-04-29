#include "hrpcheaders.h"
#include "FrameBuffer.h"

#include "Hildur/Core/Application.h"

#include <glad/glad.h>


namespace Hildur {


	const std::string DEFAULT_ATTACHMENT = "default";
	GLuint FrameBuffer::s_CurrentFBO;
	GLuint FrameBuffer::s_QuadVAO = -1;
	GLuint FrameBuffer::s_QuadVBO = -1;
	std::vector<FrameBuffer*> FrameBuffer::s_BufferList;

	std::map<FrameBuffer::InterpolationType, GLint> FrameBuffer::s_InterpolationTypes = {
		{ NEAREST , GL_NEAREST },
		{ LINEAR ,	GL_LINEAR }
	};
	std::map<FrameBuffer::InterpolationType, GLint> FrameBuffer::s_InterpolationMipTypes = {
		{ NEAREST ,			GL_NEAREST_MIPMAP_NEAREST },
		{ LINEAR ,			GL_LINEAR_MIPMAP_LINEAR },
		{ LINEAR_NEAREST ,	GL_LINEAR_MIPMAP_NEAREST},
		{ NEAREST_LINEAR ,	GL_NEAREST_MIPMAP_LINEAR }
	};
	std::map<FrameBuffer::EdgeType, GLint> FrameBuffer::s_EdgeTypes = {
		{ CLAMP_TO_EDGE ,			GL_CLAMP_TO_EDGE },
		{ CLAMP_TO_BORDER ,			GL_CLAMP_TO_BORDER },
		{ MIRRORED_REPEAT ,			GL_MIRRORED_REPEAT },
		{ REPEAT ,					GL_REPEAT },
		//{ MIRROR_CLAMP_TO_EDGE ,	GL_MIRROR_CLAMP_TO_EDGE }
	};

	//Framebuffer attachment

	FrameBuffer::BufferAttachment::BufferAttachment(FrameBuffer* frameBuffer, std::string name, bool hdr)
		: frameBuffer(frameBuffer), name(name), hdr(hdr)
	{
		rendererID = 0;
		attachmentIndex = 0;
		mipmaps = 0;
	}

	void FrameBuffer::BufferAttachment::Bind(uint32_t textureUnit)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, rendererID);
		if (mipmaps > 0)
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	//Framebuffer


	FrameBuffer::FrameBuffer(float screenFactor)
	{
		m_ColorAttachmentIndex = GL_COLOR_ATTACHMENT0;

		m_ScreenFactor = screenFactor;
		m_Width = static_cast<int>(Application::Get().GetWindow().GetWidth() * screenFactor);
		m_Height = static_cast<int>(Application::Get().GetWindow().GetHeight()* screenFactor);
		Initialize();
		s_BufferList.push_back(this);
	}

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
	{
		m_ScreenFactor = -1;
		m_Width = width;
		m_Height = height;
		Initialize();
		s_BufferList.push_back(this);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		for (auto const& ent1 : m_ColorAttachments)
		{
			glDeleteTextures(1, &ent1.second.rendererID);
		}
		glDeleteRenderbuffers(1, &m_DepthAttachment);
		s_BufferList.erase(std::remove(s_BufferList.begin(), s_BufferList.end(), this), s_BufferList.end());
	}

	void FrameBuffer::AddTextureAttachment(std::string name, bool hdr, bool mipmaps, InterpolationType interpolationType, EdgeType edgeType)
	{
		// Get reference to attachment
		BufferAttachment* temp = &m_ColorAttachments[name];

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		GLint format = hdr ? GL_RGBA16F : GL_RGBA;
		temp->frameBuffer = this;
		temp->name = name;
		temp->hdr = hdr;

		glGenTextures(1, &temp->rendererID);
		glBindTexture(GL_TEXTURE_2D, temp->rendererID);

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_EdgeTypes[edgeType]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s_EdgeTypes[edgeType]);

		if (mipmaps)
		{
			temp->mipmaps = static_cast<int>(1 + floor(log2(glm::min(m_Width, m_Height))));
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLfloat>(s_InterpolationMipTypes[interpolationType]));
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLfloat>(s_InterpolationTypes[interpolationType]));
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			temp->mipmaps = 0;
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLfloat>(s_InterpolationTypes[interpolationType]));
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLfloat>(s_InterpolationTypes[interpolationType]));
		}

		if (temp->attachmentIndex == -1)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, m_ColorAttachmentIndex, GL_TEXTURE_2D, temp->rendererID, 0);
			temp->attachmentIndex = m_ColorAttachmentIndex;
			m_DrawBuffers.push_back(m_ColorAttachmentIndex);

			++m_ColorAttachmentIndex;
		}
		else
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, temp->attachmentIndex, GL_TEXTURE_2D, temp->rendererID, 0);
		}
	}

	void FrameBuffer::AddDepthTextureAttachment(std::string name)
	{
		BufferAttachment* temp = &m_ColorAttachments[name];
		temp->frameBuffer = this;
		temp->name = name;
		temp->hdr = false;

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glGenTextures(1, &temp->rendererID);
		glBindTexture(GL_TEXTURE_2D, temp->rendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, temp->rendererID, 0);

		temp->mipmaps = 0;
		temp->attachmentIndex = GL_DEPTH_ATTACHMENT;
	}

	void FrameBuffer::AddDepthBufferAttachment()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glGenRenderbuffers(1, &m_DepthAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);
	}

	FrameBuffer::BufferAttachment* FrameBuffer::GetAttachment(std::string name) {
		return &m_ColorAttachments[name];
	}

	void FrameBuffer::Enable()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Width, m_Height);
		s_CurrentFBO = m_RendererID;
	}

	void FrameBuffer::Disable()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		s_CurrentFBO = 0;
	}

	void FrameBuffer::Bind(uint32_t textureUnit)
	{
		Bind(textureUnit, "color");
	}

	void FrameBuffer::Bind(uint32_t textureUnit, std::string name)
	{
		GetAttachment(name)->Bind(textureUnit);
	}

	void FrameBuffer::Ready()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		if (m_DrawBuffers.size() > 0)
			glDrawBuffers(static_cast<GLsizei>(m_DrawBuffers.size()), &m_DrawBuffers[0]);
		else {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			HR_CORE_WARN("Framebuffer not complete: {0}", fboStatus);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void FrameBuffer::Render(std::string name)
	{
		Render(name, nullptr);
	}

	void FrameBuffer::Render(Material* material)
	{
		Render("", material);
	}

	void FrameBuffer::Resize()
	{
		// Only recreate nonstatic buffers
		if (m_ScreenFactor == -1) return;

		m_Width = static_cast<int>(Application::Get().GetWindow().GetWidth() * m_ScreenFactor);
		m_Height = static_cast<int>(Application::Get().GetWindow().GetHeight() * m_ScreenFactor);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		if (m_DepthAttachment != -1)
		{
			glDeleteRenderbuffers(1, &m_DepthAttachment);
			AddDepthBufferAttachment();
		}

		for (auto& attachment : m_ColorAttachments)
		{
			glDeleteTextures(1, &attachment.second.rendererID);
			if (attachment.second.attachmentIndex == GL_DEPTH_ATTACHMENT)
			{
				AddDepthTextureAttachment(attachment.second.name);
			}
			else
			{
				AddTextureAttachment(attachment.second.name, attachment.second.hdr, attachment.second.mipmaps);
			}
		}

		Ready();
	}

	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		// Only recreate nonstatic buffers
		if (m_ScreenFactor == -1) return;

		m_Width = width;
		m_Height = height;
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		if (m_DepthAttachment != -1)
		{
			glDeleteRenderbuffers(1, &m_DepthAttachment);
			AddDepthBufferAttachment();
		}

		for (auto& attachment : m_ColorAttachments)
		{
			glDeleteTextures(1, &attachment.second.rendererID);
			if (attachment.second.attachmentIndex == GL_DEPTH_ATTACHMENT)
			{
				AddDepthTextureAttachment(attachment.second.name);
			}
			else
			{
				AddTextureAttachment(attachment.second.name, attachment.second.hdr, attachment.second.mipmaps);
			}
		}

		Ready();
	}

	void FrameBuffer::Blit(FrameBuffer* target, std::string sourceAttachment, std::string targetAttachment)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glReadBuffer(GetAttachment(sourceAttachment)->attachmentIndex);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target->GetFBO());
		glDrawBuffer(target->GetAttachment(targetAttachment)->attachmentIndex);

		glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, target->GetWidth(), target->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void FrameBuffer::BlitToScreen(std::string name)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glReadBuffer(GetAttachment(name)->attachmentIndex);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetWidth(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void FrameBuffer::Render(std::string name, Material* material)
	{
		glBindVertexArray(s_QuadVAO);

		Ref<Shader> shader;
		if (material == nullptr)
		{
			shader = Shader::Create("assets/shaders/FrameBufferQuad.glsl");
		}
		else
		{
			shader = material->GetShader();
		}

		shader->Bind();
		//shader->Update(nullptr);
		//shader->UpdateTextures();

		if (material != nullptr)
		{
			material->UpdateUniforms();
			//material->BindTextures();
		}
		else
		{
			bind(0, name);
		}

		//shader->enableAttributes();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//Renderer::addDrawCall();
		//shader->disableAttributes();
		shader->UnBind();

		glBindVertexArray(0);
	}

	void FrameBuffer::Initialize()
	{
		m_ColorAttachmentIndex = GL_COLOR_ATTACHMENT0;

		// Create framebuffer
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);


		// Create mesh
		if (s_QuadVAO == -1)
		{
			// Quad Mesh
			static const GLfloat _quadVertices[] = {
				-1.0f,  1.0f,
				-1.0f, -1.0f,
				1.0f,  1.0f,
				1.0f, -1.0f,
			};

			glGenVertexArrays(1, &s_QuadVAO);
			glBindVertexArray(s_QuadVAO);

			glGenBuffers(1, &s_QuadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, s_QuadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _quadVertices, GL_STATIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, s_QuadVBO);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
	}

	void FrameBuffer::ResizeAll()
	{
		for (FrameBuffer* fbo : s_BufferList)
		{
			fbo->Resize();
		}
	}

	void FrameBuffer::Destroy()
	{
		glDeleteBuffers(1, &s_QuadVBO);
		glDeleteVertexArrays(1, &s_QuadVAO);
	}

	void FrameBuffer::ResetDefaultBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetWidth());
		s_CurrentFBO = 0;
	}

	void FrameBuffer::ClearDefaultBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetWidth());
		s_CurrentFBO = 0;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}


}