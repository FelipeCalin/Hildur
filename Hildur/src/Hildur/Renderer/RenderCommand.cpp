#include "hrpcheaders.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Hildur {


	Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();


}