#include "hrpcheaders.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

#include "Renderer.h"


namespace Hildur {


	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {

		switch (Renderer::GetAPI()) {
			
			case RendererAPI::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:     return new OpenGLVertexBuffer(vertices, size);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:     return new OpenGLIndexBuffer(indices, size);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}


}