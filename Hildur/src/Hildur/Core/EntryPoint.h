#pragma once

#ifdef HR_PLATFORM_WINDOWS


extern Hildur::Application* Hildur::CreateApplication();

int main(int argc, char** argv) 
{
	Hildur::Log::Init();

	HR_CORE_INFO("Core logger initialized");

	auto app = Hildur::CreateApplication();
	app->Run();
	delete app;
}


#endif 
