#pragma once

#include <Hildur.h>

#include <GLM/glm/glm.hpp>


class Editor : public Hildur::Application
{
public:

	Editor();
	virtual ~Editor();

	void Init() override;
	void SetScenes(std::map<std::string, Hildur::Scene*>& sceneMap) override;
	void Run() override;

	void OnEvent(Hildur::Event& e);

	virtual bool IsInitOverriden() const override { return true; }

private:

	bool OnWindowResize(Hildur::WindowResizeEvent& e);
	bool OnWindowClose(Hildur::WindowCloseEvent& e);
	
	void RenderEditor();

	void DockUpdate();

	void RenderChild(Hildur::Entity* entity);

private:

	bool m_WantsToClose = false;
	bool m_CanClose = false;

	Hildur::Ref<Hildur::FrameBuffer> m_FrameBuffer;

	//Viewport
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;
	bool m_IsMovingInVP = false;

	//Hierarchy
	Hildur::Entity* m_InspectedEntity = nullptr;
	bool m_OpenRootNode = true;

	//Console
	std::vector<std::string*> m_ConsoleLog;

	//window bools

	bool m_UIShowViewPort = true;
	bool m_UIShowHierarchy = true;
	bool m_UIShowInspector = true;
	bool m_UIShowAssetExplorer = true;
	bool m_UIShowFileManager = true;

};