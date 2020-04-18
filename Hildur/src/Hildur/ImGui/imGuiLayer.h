#pragma once

#include "Hildur/Resource/Layer.h"

#include "Hildur/Events/ApplicationEvent.h"
#include "Hildur/Events/KeyEvent.h"
#include "Hildur/Events/MouseEvent.h"

namespace Hildur {


	class HILDUR_API ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	};


}