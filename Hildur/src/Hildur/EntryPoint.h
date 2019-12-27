#pragma once


#ifdef HR_PLATFORM_WINDOWS

extern Hildur::Application* Hildur::CreateApplication();

int main(int argc, char** argv) {

	printf("Hildur engine online\n");
	auto app = Hildur::CreateApplication();
	app->Run();
	delete app;

}

#endif 
