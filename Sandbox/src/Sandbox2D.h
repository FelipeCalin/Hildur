#pragma once

#include "Hildur.h"


class Sandbox2D : public Hildur::Layer
{
public:
	
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hildur::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hildur::Event& e) override;

private:

	Hildur::OrthographicCameraController m_CameraController;

	Hildur::Ref<Hildur::Texture2D> m_TextureCherno;

	float rotation = 0.0f;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};