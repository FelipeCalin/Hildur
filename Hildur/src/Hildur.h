#pragma once


//For use of applications using the engine


//---------Core Files------------------

#include "Hildur/Core/Application.h"
#include "Hildur/Resource/Layer.h"
#include "Hildur/Util/Log.h"
#include "Hildur/ImGui/ImGuiLogSink.h" // TODO: overhaul

#include "Hildur/Core/Timestep.h"

#include "Hildur/Core/Input.h"
#include "Hildur/util/KeyCodes.h"
#include "Hildur/util/MouseButtonCodes.h"

#include "Hildur/Core/Entity.h"
#include "Hildur/Resource/Scene.h"
#include "Hildur/Core/System/SceneManager.h"

#include "Hildur/OrthographicCameraController.h"
#include "Hildur/PerspectiveCameraController.h"

#include "Hildur/ImGui/imGuiLayer.h"

//--------Scene Files------------------

#include "Hildur/Resource/Scene.h"

#include "Hildur/Resource/Model.h"

//-------Component Files---------------

#include "Hildur/Component/Transform.h"
#include "Hildur/Component/Camera.h"

#include "Hildur/Component/LightEmitter.h"
#include "Hildur/Component/DirectionalLight.h"
#include "Hildur/Component/PointLight.h"

#include "Hildur/Component/Renderable.h"
#include "Hildur/Component/Sprite.h"
#include "Hildur/Component/MeshRenderer.h"

//-------Renderer Files----------------

#include "Hildur/Core/System/Renderer.h"
#include "Hildur/Core/System/Renderer2D.h"
#include "Hildur/Renderer/RenderCommand.h"

#include "Hildur/Renderer/Buffer.h"
#include "Hildur/Renderer/VertexArray.h"
#include "Hildur/Resource/FrameBuffer.h"

#include "Hildur/Resource/Shader.h"
#include "Hildur/Resource/Texture.h"

#include "Hildur/Resource/Mesh.h"

#include "Hildur/Resource/Material.h"

#include "Hildur/Renderer/OrthographicCamera.h"
#include "Hildur/Renderer/PerspectiveCamera.h"

//---------Physics---------------------

#include "Hildur/Core/System/Physics.h"

//----------Input----------------------

#include "Hildur/util/KeyCodes.h"
#include "Hildur/util/MouseButtonCodes.h"
