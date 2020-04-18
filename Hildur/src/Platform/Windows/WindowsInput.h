#pragma once

#include "Hildur/Core/Input.h"

namespace Hildur {


	class WindowsInput : public Input
	{
	protected:

		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};


}