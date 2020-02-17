#pragma once


//For use of applications using the engine


//---------Core Files------------------

#include "Hildur/Application.h"
#include "Hildur/Layer.h"
#include "Hildur/Log.h"

#include "Hildur/Core/Timestep.h"

#include "Hildur/Input.h"
#include "Hildur/KeyCodes.h"
#include "Hildur/MouseButtonCodes.h"

#include "Hildur/ImGui/imGuiLayer.h"

//--------Scene Files------------------

#include "Hildur/Scene/GameObject.h"
#include "Hildur/Scene/Scene.h"

//-------Renderer Files----------------

#include "Hildur/Renderer/Renderer.h"
#include "Hildur/Renderer/RenderCommand.h"

#include "Hildur/Renderer/Buffer.h"
#include "Hildur/Renderer/Shader.h"
#include "Hildur/Renderer/Texture.h"
#include "Hildur/Renderer/VertexArray.h"

//#include "Hildur/Renderer/PerspectiveCamera.h"
#include "Hildur/Renderer/OrthographicCamera.h"

#include "Hildur/Renderer/Primitives2D.h"

//---------Physics---------------------

//#include "Hildur/Physics/Physics.h"

//---------ECS Files-------------------

#include "Hildur/ECS/ECS.h"

//----------Input----------------------

#include "Hildur/KeyCodes.h"
#include "Hildur/MouseButtonCodes.h"

//--------EntryPoint-------------------

#include "Hildur/EntryPoint.h"
