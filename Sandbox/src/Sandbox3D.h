#pragma once

#include "Hildur.h"


class Sandbox3D : public Hildur::Layer
{
public:

	Sandbox3D();
	~Sandbox3D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hildur::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hildur::Event& e) override;

	bool OnWindowResize(Hildur::WindowResizeEvent& e);
	bool OnWindowClose(Hildur::WindowCloseEvent& e);

private:

	Hildur::ShaderLibrary m_ShaderLibrary;

	Hildur::Ref<Hildur::Texture2D> m_Texture1;
	Hildur::Ref<Hildur::Texture2D> m_Texture2;
	Hildur::Ref<Hildur::Texture2D> m_Texture3;

	Hildur::Ref<Hildur::Material> m_Material;
	Hildur::Ref<Hildur::Material> m_LightMaterial;

	//Model
	Hildur::Ref<Hildur::Model> m_Model;
	Hildur::Ref<Hildur::Model> m_CubeModel;

	Hildur::PerspectiveCameraController m_CameraController;


	glm::vec3 m_ObjectRot;
	glm::vec3 m_ObjectPos;

	float translationSpeed = 1;

	//Material Test
	float objectAmbient = 0.150f;
	float objectDiffuse = 0.40f;
	float objectSpecular = 1.5f;
	float objectShininess = 10.0f;

	glm::vec3 lightPos;
	glm::vec3 lightDir;
	glm::vec3 lightCol;
	float lightAmbient = 0.2f;
	float lightDiffuse = 0.5f;
	float lightSpecular = 1.0f;
	float lightConstant = 1.0f;
	float lightLinear = 0.045f;
	float lightQuadratic = 0.0075f;
	float lightCutOff = 5.0f;
	float lightOuterCutOff = 7.0f;

	//Framebuffer test

	Hildur::Ref<Hildur::FrameBuffer> TestFramebuffer;
};