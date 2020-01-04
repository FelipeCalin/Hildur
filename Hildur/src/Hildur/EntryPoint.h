#pragma once


#ifdef HR_PLATFORM_WINDOWS


extern Hildur::Application* Hildur::CreateApplication();

int main(int argc, char** argv) {

	int bb = 33.75;

	Hildur::Log::Init();

	HR_INFO("Client logger working, initialized with code {0}", bb);
	HR_CORE_ERROR("Core logger initialized");

	printf("Hildur engine online\n");
	auto app = Hildur::CreateApplication();
	app->Run();
	delete app;

}


#endif 
