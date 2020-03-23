#include "hrpcheaders.h"
#include "Buffer.h"

#include "Hildur/Core/System/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"


namespace Hildur {


	//Vertex Buffer//////////////////////////////////////////////////////////

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {

		switch (Renderer::GetAPI()) {

		case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLVertexBuffer>(vertices, size);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}

	Ref<VertexBuffer> VertexBuffer::Create(Ref<std::vector<Vertex>> vertices, uint32_t size) {

		switch (Renderer::GetAPI()) {
			
			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLVertexBuffer>(vertices, size);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}

	//Index Buffer//////////////////////////////////////////////////////////

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {

		switch (Renderer::GetAPI()) {

		case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLIndexBuffer>(indices, size);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}

	Ref<IndexBuffer> IndexBuffer::Create(Ref<std::vector<uint32_t>> indices, uint32_t size) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLIndexBuffer>(indices, size);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}

	//Frame Buffer//////////////////////////////////////////////////////////

	Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height, uint32_t texture) {

		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLFrameBuffer>(width, height, texture);

		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;

	}


}