#include <Hildur.h>



class ExampleLayer : public Hildur::Layer {

public:

	ExampleLayer()
		:Layer("Example") {

	}

	void OnUpdate() override {

		HR_INFO("Example layer update");

	}

	void OnEvent(Hildur::Event& e) override {

		HR_TRACE("{0}", e);

	}

};


class Sandbox : public Hildur::Application {

public:

	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Hildur::ImGuiLayer());
	}

	~Sandbox() {

	}

};

Hildur::Application* Hildur::CreateApplication() {

	return new Sandbox;

}