#include "hrpcheaders.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"


namespace Hildur {


	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;


}