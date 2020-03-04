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

	//  Frame Buffer  ////////////////////////////////////////////////////////

	class OpenGLFrameBuffer : public FrameBuffer {

	public:

		OpenGLFrameBuffer(const uint32_t width, const uint32_t height, uint32_t texture = 0);
		virtual ~OpenGLFrameBuffer();


		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual uint32_t GetID() const override { return m_RendererID; }

		virtual uint32_t GetTexture() const { return m_Texture; }
		virtual void SetTexture(Ref<Texture2D> texture) override { m_Texture = texture->GetID(); }

	private:

		uint32_t m_RendererID;
		uint32_t m_Texture;

	};


}