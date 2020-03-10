#pragma once

#include "Hildur/Renderer/RendererAPI.h"
#include "Hildur/Resource/Shader.h"

#include "Hildur/Renderer/OrthographicCamera.h"
#include "Hildur/Renderer/PerspectiveCamera.h"

#include "Hildur/Resource/Mesh.h"


namespace Hildur {


	class Renderer {

	public:

		static void Init();

		static void BeginScene(OrthographicCamera& camera); //TODO: Add camera, light, environment arguments
		static void BeginScene(PerspectiveCamera& camera); //TODO: Add camera, light, environment arguments
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));


		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:

		struct SceneData {

			glm::mat4 ViewProjectionMatrix;

		};

		static SceneData* m_SceneData;

	};


}