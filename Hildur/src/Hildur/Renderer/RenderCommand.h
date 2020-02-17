#pragma once

#include "Hildur/Renderer/RendererAPI.h"


namespace Hildur {


	class RenderCommand {

	public:

		inline static void Init() {

			s_RendererAPI->Init();

		}

		inline static void SetClearColor(const glm::vec4 color) {
		
			s_RendererAPI->SetClearColor(color);
		
		}

		inline static void Clear() {
		
			s_RendererAPI->Clear();
		
		}

		inline static void SetRenderTarget(const int& ID) {

			s_RendererAPI->SetRenderTarget(ID);

		}

		inline static int GetRenderTarget() {

			return s_RendererAPI->GetRenderTarget();

		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {

			s_RendererAPI->DrawIndexed(vertexArray);

		}

	private:

		static RendererAPI* s_RendererAPI;

	};


}