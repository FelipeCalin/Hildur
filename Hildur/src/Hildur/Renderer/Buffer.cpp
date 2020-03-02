#include "hrpcheaders.h"
#include "Buffer.h"

#include "Hildur/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"


namespace Hildur {


	VertexBuffer* VertexBuffer::Create(Ref<std::vector<Vertex>> vertices, uint32_t size) {

		switch (Renderer::GetAPI()) {
			
			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return new OpenGLVertexBuffer(vertices, size);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}

	IndexBuffer* IndexBuffer::Create(Ref<std::vector<uint32_t>> indices, uint32_t size) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return new OpenGLIndexBuffer(indices, size);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}


}