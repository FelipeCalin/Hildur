#include <Hildur.h>
#include <Hildur/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "Sandbox3D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Scene2D : public Hildur::Scene
{
public:

	void Load()
	{
		// Textures ///////////////////////////

		Hildur::Ref<Hildur::Texture2D> Texture1 = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");
		Hildur::Ref<Hildur::Texture2D> Texture2 = Hildur::Texture2D::Create("assets/textures/Fingerprints1.jpg");
		Hildur::Ref<Hildur::Texture2D> Texture3 = Hildur::Texture2D::Create("assets/textures/ShaderBallEmission.png");

		// Entities ///////////////////////////

		Hildur::Entity* cam = instantiate("Camera");
		cam->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0.5f, 0.75f, 2.0f));
		cam->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		cam->AddComponent<Hildur::Camera>();
		cam->GetComponent<Hildur::Camera>()->SetPerspectiveProjection(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		//cam->AddComponent<Hildur::StateControllerScript>();
		//cam->AddComponent<Hildur::CameraControllerScript>();

		Hildur::Entity* shaderBall = instantiate("ShaderBall");
		shaderBall->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		shaderBall->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		shaderBall->GetComponent<Hildur::Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		shaderBall->AddComponent<Hildur::Sprite>();
		shaderBall->GetComponent<Hildur::Sprite>()->SetColor(glm::vec4(0.1f, 0.6f, 0.9f, 1.0f));

	}
};

class Scene3D : public Hildur::Scene 
{
public:

	void Load()
	{
		// Skybox /////////////////////////////

		std::vector<std::string> sea = {
			"assets/skyboxes/sea/right.jpg",
			"assets/skyboxes/sea/left.jpg",
			"assets/skyboxes/sea/top.jpg",
			"assets/skyboxes/sea/bottom.jpg",
			"assets/skyboxes/sea/back.jpg",
			"assets/skyboxes/sea/front.jpg"
		};

		Hildur::Renderer::SetSkybox(Hildur::CubeMap::Create(sea));

		// Textures ///////////////////////////

		Hildur::Ref<Hildur::Texture2D> Texture1 = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");
		Hildur::Ref<Hildur::Texture2D> Texture2 = Hildur::Texture2D::Create("assets/textures/Fingerprints1.jpg");
		Hildur::Ref<Hildur::Texture2D> Texture3 = Hildur::Texture2D::Create("assets/textures/ShaderBallEmission.png");

		// Shaders ////////////////////////////

		Hildur::ShaderLibrary ShaderLibrary;
		Hildur::Ref<Hildur::Shader> PhongShader = ShaderLibrary.Load("assets/shaders/Phong.glsl");
		PhongShader->textures["albedoMap"] = 1;
		PhongShader->textures["roughnessMap"] = 2;
		Hildur::Ref<Hildur::Shader> LightShader = ShaderLibrary.Load("assets/shaders/Light.glsl");
		Hildur::Ref<Hildur::Shader> ColorShader = ShaderLibrary.Load("assets/shaders/Color.glsl");
		Hildur::Ref<Hildur::Shader> PostPShader = ShaderLibrary.Load("assets/shaders/FrameBufferQuad.glsl");

		// Materials //////////////////////////

		Hildur::Ref<Hildur::Material> Material;
		Hildur::Ref<Hildur::Material> LightMaterial;
		Material = Hildur::Material::Create(PhongShader);
		Material->SetProperty("material.ambient", glm::vec3(0.2f));
		Material->SetProperty("material.diffuse", glm::vec3(0.5f));
		Material->SetProperty("material.specular", glm::vec3(1.5f));
		Material->SetProperty("material.shininess", 10.0f);
		Material->SetProperty("albedoMap", 1U);
		Material->SetProperty("roughnessMap", 2U);
		Material->SetImageBuffer("albedoMap", Texture1);
		Material->SetImageBuffer("roughnessMap", Texture2);
		LightMaterial = Hildur::Material::Create(LightShader);

		// Models /////////////////////////////

		Hildur::Ref<Hildur::Model> Model;
		Hildur::Ref<Hildur::Model> CubeModel;
		Model = Hildur::Model::Create("assets/models/ShaderBall.fbx");
		CubeModel = Hildur::Model::Create("assets/models/cube.obj");

		// Entities ///////////////////////////

		Hildur::Entity* cam = instantiate("Camera");
		cam->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0.5f, 0.75f, 2.0f));
		cam->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		cam->AddComponent<Hildur::Camera>();
		cam->GetComponent<Hildur::Camera>()->SetPerspectiveProjection(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		//cam->AddComponent<Hildur::StateControllerScript>();
		//cam->AddComponent<Hildur::CameraControllerScript>();

		Hildur::Entity* sun = instantiate("Sun");
		sun->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		sun->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(-150.0f, -25.0f, 0.0f));
		sun->AddComponent<Hildur::DirectionalLight>();
		sun->GetComponent<Hildur::DirectionalLight>()->SetColor(glm::vec3(0.0f, 1.0f, 1.0f));
		//sun->GetComponent<Hildur::DirectionalLight>()->SetLightShader(LightShader);

		Hildur::Entity* bulb = instantiate("Bulb");
		bulb->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(-1.0f, 0.0f, 1.0f));
		bulb->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		bulb->AddComponent<Hildur::PointLight>();
		bulb->GetComponent<Hildur::PointLight>()->SetColor(glm::vec3(0.9f, 0.1f, 0.6f));
		//sun->GetComponent<Hildur::DirectionalLight>()->SetLightShader(LightShader);

		Hildur::Entity* shaderBall = instantiate("ShaderBall");
		shaderBall->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
		shaderBall->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
		shaderBall->GetComponent<Hildur::Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		shaderBall->AddComponent<Hildur::MeshRenderer>();
		shaderBall->GetComponent<Hildur::MeshRenderer>()->SetMesh(Model->GetMeshes()[0]);
		shaderBall->GetComponent<Hildur::MeshRenderer>()->SetMaterial(Material);


		Hildur::RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1 });
	}
};

class SuperScene : public Hildur::Scene 
{
public:

	void Load()
	{
		// Skybox /////////////////////////////

		std::vector<std::string> sea = {
			"assets/skyboxes/pineArches/right.png",
			"assets/skyboxes/pineArches/left.png",
			"assets/skyboxes/pineArches/top.png",
			"assets/skyboxes/pineArches/bottom.png",
			"assets/skyboxes/pineArches/back.png",
			"assets/skyboxes/pineArches/front.png"
		};

		Hildur::Renderer::SetSkybox(Hildur::CubeMap::Create(sea));

		// Textures ///////////////////////////

		Hildur::Ref<Hildur::Texture2D> Texture1 = Hildur::Texture2D::Create("assets/textures/material-ball/export3dcoat_lambert3SG_color.png");
		Hildur::Ref<Hildur::Texture2D> Texture2 = Hildur::Texture2D::Create("assets/textures/material-ball/export3dcoat_lambert3SG_gloss.png");
		Hildur::Ref<Hildur::Texture2D> Texture3 = Hildur::Texture2D::Create("assets/textures/material-ball/export3dcoat_lambert3SG_color.png");

		// Shaders ////////////////////////////

		Hildur::ShaderLibrary ShaderLibrary;
		Hildur::Ref<Hildur::Shader> PhongShader = ShaderLibrary.Load("assets/shaders/Phong.glsl");
		PhongShader->textures["albedoMap"] = 1;
		PhongShader->textures["roughnessMap"] = 2;
		Hildur::Ref<Hildur::Shader> LightShader = ShaderLibrary.Load("assets/shaders/Light.glsl");
		Hildur::Ref<Hildur::Shader> ColorShader = ShaderLibrary.Load("assets/shaders/Color.glsl");
		Hildur::Ref<Hildur::Shader> PostPShader = ShaderLibrary.Load("assets/shaders/FrameBufferQuad.glsl");

		// Materials //////////////////////////

		Hildur::Ref<Hildur::Material> Material;
		Hildur::Ref<Hildur::Material> LightMaterial;
		Material = Hildur::Material::Create(PhongShader);
		Material->SetProperty("material.ambient", glm::vec3(0.2f));
		Material->SetProperty("material.diffuse", glm::vec3(0.5f));
		Material->SetProperty("material.specular", glm::vec3(1.5f));
		Material->SetProperty("material.shininess", 10.0f);
		Material->SetProperty("albedoMap", 1U);
		Material->SetProperty("roughnessMap", 2U);
		Material->SetImageBuffer("albedoMap", Texture1);
		Material->SetImageBuffer("roughnessMap", Texture2);
		LightMaterial = Hildur::Material::Create(LightShader);

		// Models /////////////////////////////

		Hildur::Ref<Hildur::Model> CerberusModel;
		Hildur::Ref<Hildur::Model> CubeModel;
		CerberusModel = Hildur::Model::Create("assets/models/material-ball-in-3d-coat/export3dcoat.obj");
		CubeModel = Hildur::Model::Create("assets/models/cube.obj");

		// Entities ///////////////////////////

		Hildur::Entity* cam = instantiate("Camera");
		cam->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0.5f, 0.1f, 2.0f));
		cam->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		cam->AddComponent<Hildur::Camera>();
		cam->GetComponent<Hildur::Camera>()->SetPerspectiveProjection(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		//cam->AddComponent<Hildur::StateControllerScript>();
		//cam->AddComponent<Hildur::CameraControllerScript>();

		Hildur::Entity* sun = instantiate("Sun");
		sun->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(-2.f, 0.0f, -2.0f));
		sun->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(-190.0f, 30.0f, 0.0f));
		sun->AddComponent<Hildur::DirectionalLight>();
		sun->GetComponent<Hildur::DirectionalLight>()->SetColor(glm::vec3(1.0f, 0.9f, 0.9f));
		sun->GetComponent<Hildur::DirectionalLight>()->SetLightShader(LightShader);

		Hildur::Entity* shaderBall = instantiate("ShaderBall");
		shaderBall->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0, 0, 1));
		shaderBall->GetComponent<Hildur::Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		shaderBall->AddComponent<Hildur::MeshRenderer>();
		shaderBall->GetComponent<Hildur::MeshRenderer>()->SetMesh(CerberusModel->GetMeshes()[0]);
		shaderBall->GetComponent<Hildur::MeshRenderer>()->SetMaterial(Material);

		Hildur::Entity* cube = instantiate("cube");
		cube->GetComponent<Hildur::Transform>()->SetParent(shaderBall);
		cube->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(1, 0, 1));
		cube->GetComponent<Hildur::Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		cube->AddComponent<Hildur::MeshRenderer>();
		cube->GetComponent<Hildur::MeshRenderer>()->SetMesh(CubeModel->GetMeshes()[0]);
		cube->GetComponent<Hildur::MeshRenderer>()->SetMaterial(Material);

		Hildur::RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1 });
	}
};



class Sandbox : public Hildur::Application 
{
public:

	Sandbox() 
	{
		std::map<std::string, Hildur::Scene*> scenes;
		scenes["Scene 2D"] = new Scene2D();
		//scenes["ImaginaryScene"] = new ImaginaryScene();
		scenes["SuperScene"] = new SuperScene();
		scenes["Scene 3D"] = new Scene3D();

		Init();

		SetScenes(scenes);

		//PushLayer(new Sandbox2D());
		//PushLayer(new Sandbox3D());
	}

	~Sandbox() 
	{
	}
};

Hildur::Application* Hildur::CreateApplication()
{
	return new Sandbox;
}