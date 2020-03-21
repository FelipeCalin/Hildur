#pragma once

#include "Hildur/Resource/Texture.h"

#include <glm/glm.hpp>


namespace Hildur {


	enum class ShaderDataType {

		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool

	};

	struct Vertex {

		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;

	};


	static uint32_t ShaderDataTypeSize(ShaderDataType type) {

		switch (type) {

			case ShaderDataType::Float:     return 4;
			case ShaderDataType::Float2:    return 4 * 2;
			case ShaderDataType::Float3:    return 4 * 3;
			case ShaderDataType::Float4:    return 4 * 4;
			case ShaderDataType::Int:       return 4;
			case ShaderDataType::Int2:      return 4 * 2;
			case ShaderDataType::Int3:      return 4 * 3;
			case ShaderDataType::Int4:      return 4 * 4;
			case ShaderDataType::Mat3:      return 4 * 3 * 3;
			case ShaderDataType::Mat4:      return 4 * 4 * 4;
			case ShaderDataType::Bool:      return 1;

		}

		HR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;

	}

	struct BufferElement {

		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;


		BufferElement() {};

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {

		}

		uint32_t GetComponentCount() const {

			switch (Type) {

				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;

			}

			HR_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;

		}

	};

	class BufferLayout {

	public:

		BufferLayout() {};
		BufferLayout(const std::initializer_list<BufferElement>& elements) 
			:m_Elements(elements) {
		
			CalculateOffsetAndStride();

		};

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:

		void CalculateOffsetAndStride() {

			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& elements : m_Elements) {

				elements.Offset = offset;
				offset += elements.Size;
				m_Stride += elements.Size;

			}

		}

	private:

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;

	};

	class VertexBuffer {

	public:

		virtual ~VertexBuffer() {}

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Ref<VertexBuffer> Create(Ref<std::vector<Vertex>> vertices, uint32_t size);

	};

	class IndexBuffer {

	public:

		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(Ref<std::vector<uint32_t>> indices, uint32_t size);

	};

	class FrameBuffer {

	public:

		virtual ~FrameBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetID() const = 0;

		virtual uint32_t GetTexture() const = 0;
		virtual void SetTexture(Ref<Texture2D> texture) = 0;



		static Ref<FrameBuffer> Create(const uint32_t width, const uint32_t height, uint32_t texture = 0);

	};


}