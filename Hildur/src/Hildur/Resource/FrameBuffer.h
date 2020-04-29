#pragma once

#include "Hildur/Resource/ImageBuffer.h"
#include "Hildur/Resource/Material.h"


namespace Hildur {


	class FrameBuffer : public ImageBuffer
	{
	public:

		static const std::string DEFAULT_ATTACHMENT;

		struct BufferAttachment : public ImageBuffer
		{
			BufferAttachment() {}
			BufferAttachment(FrameBuffer* frameBuffer, std::string name, bool hdr);

			void Enable() override {}
			void Bind(uint32_t textureUnit) override;
			
			std::string name;
			bool hdr;
			int mipmaps;
			uint32_t rendererID;
			uint32_t attachmentIndex = -1;
			FrameBuffer* frameBuffer;
		};

		enum InterpolationType
		{
			NEAREST,
			LINEAR,
			LINEAR_NEAREST,
			NEAREST_LINEAR
		};

		enum EdgeType
		{
			CLAMP_TO_EDGE,
			CLAMP_TO_BORDER,
			MIRRORED_REPEAT,
			REPEAT,
			MIRROR_CLAMP_TO_EDGE
		};

		FrameBuffer(float screenFactor);

		FrameBuffer(uint32_t width, uint32_t height);
		~FrameBuffer();

		void AddTextureAttachment(std::string name, bool hdr = false, bool mipmaps = false, InterpolationType interpolationType = LINEAR, EdgeType edgeType = CLAMP_TO_EDGE);
		void AddDepthTextureAttachment(std::string name);
		void AddDepthBufferAttachment();
		BufferAttachment* GetAttachment(std::string name);

		void Enable();
		void Disable();

		void Bind(uint32_t textureUnit);
		void Bind(uint32_t textureUnit, std::string name);

		void Ready();

		void Clear(float r = 0, float g = 0, float b = 0, float a = 0);
		void Render(std::string name);
		void Render(Material* material);
		void Resize();
		void Resize(uint32_t width, uint32_t height);

		void Blit(FrameBuffer* target, std::string sourceAttachment, std::string targetAttachment);
		void BlitToScreen(std::string name);
		uint32_t GetTextureID(std::string name) { return m_ColorAttachments[name].rendererID; }
		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }
		uint32_t GetFBO() { return m_RendererID; }

		static void ResetDefaultBuffer();
		static void ClearDefaultBuffer();
		static void ResizeAll();
		static void Destroy();

		static Ref<FrameBuffer> Create(float screenFactor)
		{
			return CreateRef<FrameBuffer>(screenFactor);
		}

		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height)
		{
			return CreateRef<FrameBuffer>(width, height);
		}

	private:

		// config
		uint32_t m_Width;
		uint32_t m_Height;
		float m_ScreenFactor;

		// Buffers
		uint32_t m_RendererID;
		uint32_t m_DepthAttachment = -1;
		std::unordered_map<std::string, BufferAttachment> m_ColorAttachments;

		uint32_t m_ColorAttachmentIndex;
		std::vector<uint32_t> m_DrawBuffers;

		//Opengl Wrapper
		static std::map<InterpolationType, int32_t> s_InterpolationTypes;
		static std::map<InterpolationType, int32_t> s_InterpolationMipTypes;
		static std::map<EdgeType, int32_t> s_EdgeTypes;

		// Mesh
		static uint32_t s_QuadVAO;
		static uint32_t s_QuadVBO;

		static uint32_t s_CurrentFBO;
		static std::vector<FrameBuffer*> s_BufferList;

		void Render(std::string name, Material* material);
		void Initialize();

	};


}