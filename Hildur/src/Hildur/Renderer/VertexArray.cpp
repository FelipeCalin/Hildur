#include "hrpcheaders.h"
#include "VertexArray.h"

#include "Hildur/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"


namespace Hildur {


	VertexArray* VertexArray::Create() {

		switch (Renderer::GetAPI()) {

		case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:     return new OpenGLVertexArray();

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}


}