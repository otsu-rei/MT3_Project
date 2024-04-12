#pragma once
#include <Novice.h>

// Lib > Geometry //
#include "MyVector2.h"

class InputManager final {
private:

	// keys
	static char keys_[256];
	static char preKeys_[256];

	// mouse
	static Vector2i preMousePos_;
	static Vector2i mousePos_;

public:

	InputManager();
	~InputManager();

	static void Update();

	// keys
	static bool IsTriggerKeys(int dik);
	static bool IsPressKeys(int dik);

	// mouse
	static Vector2i GetMousePos();
	static Vector2i GetPreMousePos();
	static bool IsTriggerMouse(int buttonNumber);
	static bool IsPressMouse(int buttonNumber);
	static int GetWheel();
};