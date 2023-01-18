#pragma once

#include "Input.h"

class WindowsInput : public Input {
protected:
	virtual bool IsKeyPressedImpl(int keycode) override;

	virtual bool IsMouseButtonPressedImpl(int button) override;
	virtual float GetMouseXImpl() override;
	virtual float GetMouseYImpl() override;
	virtual Vector2::Double GetMousePositionImpl() override;
};