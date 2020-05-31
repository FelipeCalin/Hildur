#pragma once

#include "Hildur/Renderer/RendererAPI.h"
#include "Hildur/Resource/Shader.h"
#include "Hildur/Resource/Material.h"
#include "Hildur/Resource/CubeMap.h"

#include "Hildur/Component/LightEmitter.h"

#include "Hildur/Core/Entity.h"

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

		static void Prep();
		static void RenderQueue();
		static void End();

		static void RenderID();
		static void RenderGizmos();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Shader>& shader, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Material>& material, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));

		static Ref<CubeMap> GetSkybox();
		static void SetSkybox(Ref<CubeMap> cubemap);

		static void AddToRenderQueue(Renderable* renderable);
		static void RemoveFromRenderQueue(Renderable* renderable);

		static void AddToLightList(LightEmitter* light);
		static void RemoveFromLightList(LightEmitter* light);

		static Hildur::Entity* GetEntityFromID(uint32_t id);

		static const glm::vec3& GetCameraPos();
		static const glm::mat4& GetViewMat();
		static const glm::mat4& GetProjectionMat();
		static const glm::mat4& GetViewProjectionMat();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};


}