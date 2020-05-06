#include "hrpcheaders.h"
#include "PointLight.h"

#include "Hildur/Core/System/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Hildur {


	void PointLight::Init()
	{
		m_Type = LE_POINT;

		Renderer::AddToLightList(this);

		m_LightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
	}

	PointLight::~PointLight()
	{
		//shadowBuffer->clear();
	}

	void PointLight::Destroy()
	{
		//Lighting::removeLight(this);
		Renderer::RemoveFromLightList(this);
	}

	/*
	void DirectionalLight::renderShadowMap(bool captureMode)
	{
		shadowBuffer->enable();
		shadowBuffer->clear();

		//Renderer::render(_depthShader, captureMode);


		if(!captureMode){
			ImGui::Begin("Depth");
			ImGui::Image((ImTextureID)shadowBuffer->getAttachment("depth")->id, ImVec2(_shadowMapResolution/15.0f, _shadowMapResolution/15.0f), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
		}



		//FrameBuffer::resetDefaultBuffer();
	}
	*/


	void PointLight::OnEnable()
	{
		//Lighting::addLight(this);
		Renderer::AddToLightList(this);
	}

	void PointLight::OnDisable()
	{
		//Lighting::removeLight(this);
		Renderer::RemoveFromLightList(this);
	}


}