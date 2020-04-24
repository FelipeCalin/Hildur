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

private:

	Hildur::ShaderLibrary m_ShaderLibrary;

	Hildur::Ref<Hildur::Texture2D> m_Texture;
	Hildur::Ref<Hildur::Texture2D> m_Texture2;

	Hildur::Ref<Hildur::Material> m_Material;
	Hildur::Ref<Hildur::Material> m_FlatColorMaterial;

	//Hildur::Ref<Hildur::Mesh> m_Mesh;

	Hildur::PerspectiveCameraController m_CameraController;


	glm::vec3 m_ObjectRot;
	glm::vec3 m_ObjectPos;

	float translationSpeed = 1;

	glm::vec3 LightPos;
	glm::vec3 LightColor;

	//Material Test
	float ambient = 0.150f;
	float diffuse = 6.5f;
	float specular =1.5f;
	

	//Model
	Hildur::Ref<Hildur::Model> m_Model;
	Hildur::Ref<Hildur::Model> m_CubeMesh;
};