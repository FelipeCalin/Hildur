#include "hrpcheaders.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

#include "glad/glad.h"
#include <gl/GL.h>


namespace Hildur {


	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HR_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init() {

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)wglGetProcAddress);
		HR_CORE_ASSERT(status, "Falied to initialize Glad!");


		//Gl Checking

		int minor, major = 0;

		glGetIntegerv(GL_MINOR_VERSION, &minor);
		glGetIntegerv(GL_MAJOR_VERSION, &major);

		HR_CORE_INFO("OpenGL Minimum version is: {0}", minor);
		HR_CORE_INFO("OpenGl Maximum version is: {0}", major);
		HR_CORE_INFO("Graphical vendor is: {0}", glGetString(GL_VENDOR));
		HR_CORE_INFO("Graphical Renderer name is: {0}", glGetString(GL_RENDERER));
		HR_CORE_INFO("Shading lenguage version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

	}

	void OpenGLContext::SwapBuffers() {

		glfwSwapBuffers(m_WindowHandle);

	}


}