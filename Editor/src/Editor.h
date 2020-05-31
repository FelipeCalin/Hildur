#pragma once

#include <Hildur.h>

#include "ImGuiWidgets/FileExplorer.h"
#include "ImGuiWidgets/Viewport.h"


#include <GLM/glm/glm.hpp>


namespace Editor {


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
		bool OnWindowMove(Hildur::WindowMoveEvent& e);
		bool OnWindowClose(Hildur::WindowCloseEvent& e);

		bool OnMouseClick(Hildur::MouseButtonPressedEvent& e);
		bool OnMouseScrolled(Hildur::MouseScrolledEvent& e);

		void RenderEditor();

		void DockUpdate();

		void RenderChild(Hildur::Entity* entity);

	private:

		bool m_WantsToClose = false;
		bool m_CanClose = false;

		Hildur::Ref<Hildur::FrameBuffer> m_FrameBuffer;

		//Viewport
		Viewport m_Viewport;
		//Test
		Hildur::Entity* m_SelectedEntity;

		//Hierarchy
		Hildur::Entity* m_InspectedEntity = nullptr;
		bool m_OpenRootNode = true;

		//Inspector
		bool m_ExploreFSInspector = false;
		imgui_ext::file_browser_modal m_InspectorFileBrowser = imgui_ext::file_browser_modal("Import model");

		//Asset manager
		bool m_IsImportClicked = false;
		imgui_ext::file_browser_modal fileBrowser = imgui_ext::file_browser_modal("Import");

		//Console
		std::shared_ptr<Hildur::ImGuiConsole> m_Console;

		//window bools

		bool m_UIShowViewPort = true;
		bool m_UIShowHierarchy = true;
		bool m_UIShowInspector = true;
		bool m_UIShowConsole = true;
		bool m_UIShowAssetExplorer = true;
		bool m_UIShowFileManager = true;

	};


}