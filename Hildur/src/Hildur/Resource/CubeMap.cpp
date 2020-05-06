#include "hrpcheaders.h"
#include "CubeMap.h"

#include "Hildur/Core/System/Renderer.h"

#include "Platform/OpenGL/OpenGLCubeMap.h"


namespace Hildur {


	Ref<CubeMap> CubeMap::Create(std::vector<std::string> paths)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:       HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:     return std::make_shared<OpenGLCubeMap>(paths);
		}

		HR_CORE_ASSERT(false, "Unknown RenderAPI!");

		return nullptr;
	}

	
}