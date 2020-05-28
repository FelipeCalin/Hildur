#include "hrpcheaders.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>


namespace Hildur {


#if 0
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         HR_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       HR_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          HR_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: HR_CORE_TRACE(message); return;
		}

		HR_CORE_ASSERT(false, "Unknown severity level!");
	}
#endif

	void OpenGLRendererAPI::Init()
	{
		HR_PROFILE_FUNCTION();
		
#ifdef HR_DEBUG
#if 0
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
#endif

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