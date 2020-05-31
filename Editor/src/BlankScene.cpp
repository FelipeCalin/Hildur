#include "BlankScene.h"


void BlankScene::Load()
{
	// Skybox /////////////////////////////

	//std::vector<std::string> sea = {
	//	"assets/skyboxes/pineArches/right.png",
	//	"assets/skyboxes/pineArches/left.png",
	//	"assets/skyboxes/pineArches/top.png",
	//	"assets/skyboxes/pineArches/bottom.png",
	//	"assets/skyboxes/pineArches/back.png",
	//	"assets/skyboxes/pineArches/front.png"
	//};

	//Hildur::Renderer::SetSkybox(Hildur::CubeMap::Create(sea));

	// Textures ///////////////////////////
	
	//Hildur::Ref<Hildur::Texture2D> Texture1 = Hildur::Texture2D::Create("assets/textures/Checkerboard.png");
	Hildur::Ref<Hildur::Texture2D> Texture1 = Hildur::Texture2D::Create("assets/textures/AriasBG.PNG");
	Hildur::Ref<Hildur::Texture2D> Texture2 = Hildur::Texture2D::Create("assets/textures/Fingerprints1.jpg");
	Hildur::Ref<Hildur::Texture2D> Texture3 = Hildur::Texture2D::Create("assets/textures/ShaderBallEmission.png");
	Hildur::Ref<Hildur::Texture2D> Texture4 = Hildur::Texture2D::Create("assets/textures/grid.png");

	// Shaders ////////////////////////////

	Hildur::ShaderLibrary ShaderLibrary;
	Hildur::Ref<Hildur::Shader> PhongShader = ShaderLibrary.Load("assets/shaders/Phong.glsl");
	PhongShader->textures["albedoMap"] = 2;
	PhongShader->textures["roughnessMap"] = 3;
	Hildur::Ref<Hildur::Shader> LightShader = ShaderLibrary.Load("assets/shaders/Light.glsl");
	Hildur::Ref<Hildur::Shader> ColorShader = ShaderLibrary.Load("assets/shaders/Color.glsl");
	Hildur::Ref<Hildur::Shader> TextureShader = ShaderLibrary.Load("assets/shaders/Texture.glsl");
	TextureShader->textures["u_Texture"] = 4;
	Hildur::Ref<Hildur::Shader> PostPShader = ShaderLibrary.Load("assets/shaders/FrameBufferQuad.glsl");

	// Materials //////////////////////////

	Hildur::Ref<Hildur::Material> Material;
	Material = Hildur::Material::Create(PhongShader);
	Material->SetProperty("material.ambient", glm::vec3(0.2f));
	Material->SetProperty("material.diffuse", glm::vec3(0.5f));
	Material->SetProperty("material.specular", glm::vec3(1.5f));
	Material->SetProperty("material.shininess", 10.0f);
	Material->SetProperty("albedoMap", 2U);
	Material->SetProperty("roughnessMap", 3U);
	Material->SetImageBuffer("albedoMap", Texture1);
	Material->SetImageBuffer("roughnessMap", Texture2);
	Hildur::Ref<Hildur::Material> GridMaterial;
	GridMaterial = Hildur::Material::Create(TextureShader);
	GridMaterial->SetProperty("u_Texture", 4U);
	GridMaterial->SetImageBuffer("u_Texure", Texture4);
	GridMaterial->SetProperty("u_TilingFactor", 1.0f);
	GridMaterial->SetProperty("u_Color", glm::vec3(1.0f));
	Hildur::Ref<Hildur::Material> LightMaterial;
	LightMaterial = Hildur::Material::Create(LightShader);

	// Models /////////////////////////////

	Hildur::Ref<Hildur::Model> Model;
	Hildur::Ref<Hildur::Model> CubeModel;
	Hildur::Ref<Hildur::Model> QuadModel;
	Model = Hildur::Model::Create("assets/models/ShaderBall.fbx");
	CubeModel = Hildur::Model::Create("assets/models/cube.obj");
	QuadModel = Hildur::Model::Create("assets/models/quad.obj");

	// Entities ///////////////////////////

	Hildur::Entity* cam = instantiate("Camera");
	cam->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0.5f, 0.75f, 2.0f));
	cam->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	cam->AddComponent<Hildur::Camera>();
	cam->GetComponent<Hildur::Camera>()->SetPerspectiveProjection(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
	//cam->AddComponent<Hildur::StateControllerScript>();
	//cam->AddComponent<Hildur::CameraControllerScript>();

	//Hildur::Entity* grid = instantiate("Grid");
	//grid->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//grid->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	//grid->GetComponent<Hildur::Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	//grid->AddComponent<Hildur::MeshRenderer>();
	//grid->GetComponent<Hildur::MeshRenderer>()->SetMesh(QuadModel->GetMeshes()[0]);
	//grid->GetComponent<Hildur::MeshRenderer>()->SetMaterial(GridMaterial);

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

	//Hildur::Entity* leCube = instantiate("LeCube");
	//leCube->GetComponent<Hildur::Transform>()->SetPosition(glm::vec3(0.0f, 0.0f, -1.0f));
	//leCube->GetComponent<Hildur::Transform>()->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	//leCube->GetComponent<Hildur::Transform>()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	//leCube->GetComponent<Hildur::Transform>()->SetParent(shaderBall);
	//leCube->AddComponent<Hildur::MeshRenderer>();
	//leCube->GetComponent<Hildur::MeshRenderer>()->SetMesh(CubeModel->GetMeshes()[0]);
	//leCube->GetComponent<Hildur::MeshRenderer>()->SetMaterial(Material);


	Hildur::RenderCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1 });
}