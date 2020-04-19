#pragma once

#ifdef HR_PLATFORM_WINDOWS


extern Hildur::Application* Hildur::CreateApplication();

int main(int argc, char** argv) 
{
	Hildur::Log::Init();

	HR_PROFILE_BEGIN_SESSION("Startup", "HildurProfile-Startup.json");
	auto app = Hildur::CreateApplication();
	HR_PROFILE_END_SESSION();

	HR_PROFILE_BEGIN_SESSION("Runtime", "HildurProfile-Runtime.json");
	app->Run();
	HR_PROFILE_END_SESSION();

	HR_PROFILE_BEGIN_SESSION("Shutdown", "HildurProfile-Shutdown.json");
	delete app;
	HR_PROFILE_END_SESSION();
}


#endif 
