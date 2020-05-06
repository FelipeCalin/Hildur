#include "hrpcheaders.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>


namespace Hildur {


	void OpenGLRendererAPI::Init()
	{
		HR_PROFILE_FUNCTION();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4 color) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetRenderTarget(const int& ID) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		RendererAPI::m_TargetID = ID;
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) 
	{
		HR_PROFILE_RENDERER_FUNCTION()

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


}