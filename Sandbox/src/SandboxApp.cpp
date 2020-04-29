#include <Hildur.h>
#include <Hildur/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "Sandbox3D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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

class Scene3D : public Hildur::Scene 
{
public:

	void Load()
	{
		// Textures ///////////////////////////

		Hildur::Ref<Hildur::Texture2D> Texture1 = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");
		Hildur::Ref<Hildur::Texture2D> Texture2 = Hildur::Texture2D::Create("assets/textures/Fingerprints1.jpg");
		Hildur::Ref<Hildur::Texture2D> Texture3 = Hildur::Texture2D::Create("assets/textures/ShaderBallEmission.png");

		// Shaders ////////////////////////////

		Hildur::ShaderLibrary ShaderLibrary;
		Hildur::Ref<Hildur::Shader> phongShader = ShaderLibrary.Load("assets/shaders/Phong.glsl");
		Hildur::Ref<Hildur::Shader> lightShader = ShaderLibrary.Load("assets/shaders/Light.glsl");
		Hildur::Ref<Hildur::Shader> colorShader = ShaderLibrary.Load("assets/shaders/Color.glsl");
		Hildur::Ref<Hildur::Shader> depthShader = ShaderLibrary.Load("assets/shaders/Depth.glsl");

		// Materials //////////////////////////

		Hildur::Ref<Hildur::Material> Material;
		Hildur::Ref<Hildur::Material> LightMaterial;
		Material = Hildur::Material::Create(phongShader);
		Material->SetProperty("albedoMap", 1U);
		Material->SetProperty("roughnessMap", 2U);
		LightMaterial = Hildur::Material::Create(lightShader);

		// Textures ///////////////////////////

		Hildur::Ref<Hildur::Model> Model;
		Hildur::Ref<Hildur::Model> CubeModel;
		Model = Hildur::Model::Create("assets/models/shaderball.obj");
		CubeModel = Hildur::Model::Create("assets/models/cube.obj");

		// Models /////////////////////////////

		Hildur::Entity* cam = instantiate("Camera");
		cam->AddComponent<Hildur::Camera>();
		cam->GetComponent<Hildur::Camera>()->SetPerspectiveProjection(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		cam->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0, 0, -10.0f));
		cam->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0, 0, 0));
		//cam->AddComponent<Hildur::StateControllerScript>();
		//cam->AddComponent<Hildur::CameraControllerScript>();

		Hildur::Entity* sun = instantiate("Sun");
		sun->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(-2.f, 0.0f, -2.0f));
		sun->AddComponent<Hildur::DirectionalLight>()->SetDirection(glm::vec3(0.1f, 0.6f, 0.9f));
		sun->GetComponent<Hildur::DirectionalLight>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		sun->GetComponent<Hildur::DirectionalLight>()->SetLightShader(lightShader);

		Hildur::Entity* shaderBall = instantiate("ShaderBall");
		shaderBall->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0, 0, 1));
		shaderBall->AddComponent<Hildur::MeshRenderer>()->SetMesh(Model->GetMeshes()[0]);


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
		//scenes["Scene 3D"] = new Scene3D();

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