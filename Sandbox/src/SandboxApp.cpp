#include <Hildur.h>

class Sandbox : public Hildur::Application {

public:

	Sandbox() {

	}

	~Sandbox() {

	}

};

Hildur::Application* Hildur::CreateApplication() {

	return new Sandbox;

}