#include "hrpcheaders.h"
#include "Texture.h"

#include "Hildur/Core/System/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"


namespace Hildur {


	Ref<Texture2D> Texture2D::Create(const std::string& path) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:    HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);

		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;

	}

	Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height, const uint32_t channels) {

		switch (Renderer::GetAPI()) {

		case RendererAPI::API::None:    HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(width, height, channels);

		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;

	}


}