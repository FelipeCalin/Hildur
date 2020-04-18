#pragma once

#include <string>

#include "Hildur/Core/Core.h"


namespace Hildur {

	enum TextureType {

		HR_DIFFUSE = 0,
		HR_SPECULAR = 1

	};

	class Texture {

	public:

		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetID() const = 0;

	};

	class Texture2D : public Texture {

	public:

		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);

		TextureType type;
		uint32_t id;
		std::string path;

	};

}