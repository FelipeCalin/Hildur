#include <Hildur.h>
#include <Hildur/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"
#include "Sandbox3D.h"



class ImaginaryScene : public Hildur::Scene 
{
public:

	void Load() 
	{
		Hildur::Entity* cam = instantiate("Camera");
		cam->AddComponent<Hildur::Camera>();
		cam->GetComponent<Hildur::Camera>()->SetOrhographicProjection(45.0f, 0.1f, 100.0f);
		cam->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0, 0, 0));
		cam->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0, 0, 0));

		Hildur::Entity* mike = Scene::instantiate("manuel");
		mike->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(8.0f, 3.0f, 6.0f));

		Hildur::RenderCommand::SetClearColor({ 0.8f, 0.3f, 0.3f, 1 });
	}
};

class SuperScene : public Hildur::Scene 
{
public:

	void Load()
	{
		Hildur::Entity* cam = instantiate("Camera");
		cam->AddComponent<Hildur::Camera>();
		cam->GetComponent<Hildur::Camera>()->SetOrhographicProjection(45.0f, 0.1f, 100.0f);
		cam->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0, 0, 0));
		cam->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0, 0, 0));
	
		Hildur::Entity* manuel = Scene::instantiate("manuel");
		manuel->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(1.0f, 12.0f, 5.0f));

		Hildur::RenderCommand::SetClearColor({ 0.1f, 0.5f, 0.9f, 1 });
	}
};

class MegaScene : public Hildur::Scene 
{
public:

	void Load()
	{
		Hildur::Ref<Hildur::Texture2D> checkerTexture = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");

		Hildur::Entity* cam = instantiate("Camera");
		cam->AddComponent<Hildur::Camera>();
		cam->GetComponent<Hildur::Camera>()->SetOrhographicProjection(45.0f, 0.1f, 100.0f);
		cam->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0, 0, 0));
		cam->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0, 0, 0));
		//cam->AddComponent<Hildur::StateControllerScript>();
		//cam->AddComponent<Hildur::CameraControllerScript>();

		Hildur::Entity* dexter = instantiate("Dexter");
		dexter->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0, 0, -1));
		dexter->AddComponent<Hildur::Sprite>();
		//dexter->GetComponent<Hildur::Sprite>()->SetTexture(checkerTexture);
		dexter->GetComponent<Hildur::Sprite>()->SetColor({1.0f, 1.0f, 1.0f, 1.0f});

		Hildur::RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1 });
	}
};



class Sandbox : public Hildur::Application 
{
public:

	Sandbox() 
	{
		//std::map<std::string, Hildur::Scene*> scenes;
		//scenes["ImaginaryScene"] = new ImaginaryScene();
		//scenes["SuperScene"] = new SuperScene();
		//scenes["MegaScene"] = new MegaScene();

		//Init(scenes);

		//PushLayer(new Sandbox2D());
		PushLayer(new Sandbox3D());
	}

	~Sandbox() 
	{
	}
};

Hildur::Application* Hildur::CreateApplication()
{
	return new Sandbox;
}