#pragma once

#include "Hildur/Renderer/VertexArray.h"

#include <glm/glm.hpp>


namespace Hildur {


	class RendererAPI {

	public:

		enum class API {

			None = 0,
			OpenGL = 1

		};

	public:

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const glm::vec4 color) = 0;
		virtual void Clear() = 0;

		virtual void SetRenderTarget(const int& ID) = 0;
		virtual int GetRenderTarget() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }

	private:

		static API s_API;

	protected:

		static int m_TargetID;

	};


}