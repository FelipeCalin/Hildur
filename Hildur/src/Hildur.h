#pragma once


//For use of applications using the engine


//---------Core Files------------------

#include "Hildur/Application.h"
#include "Hildur/Layer.h"
#include "Hildur/util/Log.h"

#include "Hildur/Core/Timestep.h"

#include "Hildur/Input.h"
#include "Hildur/util/KeyCodes.h"
#include "Hildur/util/MouseButtonCodes.h"

#include "Hildur/OrthographicCameraController.h"
#include "Hildur/PerspectiveCameraController.h"

#include "Hildur/ImGui/imGuiLayer.h"

//--------Scene Files------------------

//#include "Hildur/Resource/Scene.h"

#include "Hildur/Resource/Model.h"

//-------Renderer Files----------------

#include "Hildur/Renderer/Renderer.h"
#include "Hildur/Renderer/RenderCommand.h"

#include "Hildur/Renderer/Buffer.h"
#include "Hildur/Renderer/VertexArray.h"

#include "Hildur/Resource/Shader.h"
#include "Hildur/Resource/Texture.h"

#include "Hildur/Resource/Mesh.h"

#include "Hildur/Resource/Material.h"

#include "Hildur/Renderer/OrthographicCamera.h"
#include "Hildur/Renderer/PerspectiveCamera.h"

//---------Physics---------------------

//#include "Hildur/Physics/Physics.h"

//---------ECS Files-------------------

#include "Hildur/Component/ECS/ECS.h"

//----------Input----------------------

#include "Hildur/util/KeyCodes.h"
#include "Hildur/util/MouseButtonCodes.h"

//--------EntryPoint-------------------

#include "Hildur/EntryPoint.h"
