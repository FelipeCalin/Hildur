#include "hrpcheaders.h"
#include "RendererAPI.h"


namespace Hildur {


	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
	int RendererAPI::m_TargetID = 0;


}