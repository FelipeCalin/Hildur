#pragma once

#include "Hildur/Renderer/Buffer.h"
#include "Hildur/Renderer/Mesh.h"


namespace Hildur {

	//  Vertex Buffer  ///////////////////////////////////////////////////////

	class OpenGLVertexBuffer : public VertexBuffer {

	public:

		OpenGLVertexBuffer(Ref<std::vector<Vertex>> vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();


		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:

		uint32_t m_RendererID;
		BufferLayout m_Layout;

	};


	//  Index Buffer  ////////////////////////////////////////////////////////

	class OpenGLIndexBuffer : public IndexBuffer {

	public:

		OpenGLIndexBuffer(Ref<std::vector<uint32_t>> indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();


		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual uint32_t GetCount() const { return m_Count; }

	private:

		uint32_t m_RendererID;
		uint32_t m_Count;

	};


}