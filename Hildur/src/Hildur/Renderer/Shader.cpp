#include "hrpcheaders.h"
#include "Shader.h"

#include "Hildur/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Hildur {


	Shader* Shader::Create(const std::string& filepath) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return new OpenGLShader(filepath);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return new OpenGLShader(vertexSrc, fragmentSrc);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}


}