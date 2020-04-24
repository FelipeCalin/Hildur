#pragma once

#include "Hildur/Renderer/RendererAPI.h"
#include "Hildur/Resource/Shader.h"
#include "Hildur/Resource/Material.h"

#include "Hildur/Renderer/OrthographicCamera.h"
#include "Hildur/Renderer/PerspectiveCamera.h"

#include "Hildur/Resource/Mesh.h"


namespace Hildur {


	class Renderable;

	enum RenderType
	{
		RT_OPAQUE, RT_CUTOUT, RT_TRANSPARENT
	};

	class Renderer 
	{
	public:

		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera); //TODO: Add camera, light, environment arguments
		static void BeginScene(PerspectiveCamera& camera); //TODO: Add camera, light, environment arguments
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));

		static void AddToRenderQueue(Renderable* renderable);
		static void RemoveFromRenderQueue(Renderable* renderable);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	};


}