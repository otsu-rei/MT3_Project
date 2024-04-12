#include "InputManager.h"

char InputManager::keys_[256] = { 0 };
char InputManager::preKeys_[256] = { 0 };

Vector2i InputManager::mousePos_ = { 0 };
Vector2i InputManager::preMousePos_ = { 0 };

InputManager::InputManager() {
}

InputManager::~InputManager() {
}

void InputManager::Update() {
	memcpy(preKeys_, keys_, 256);
	Novice::GetHitKeyStateAll(keys_);

	preMousePos_ = mousePos_;
	Novice::GetMousePosition(&mousePos_.x, &mousePos_.y);
}

bool InputManager::IsTriggerKeys(int dik) {
	return keys_[dik] && !preKeys_[dik];
}

bool InputManager::IsPressKeys(int dik) {
	return keys_[dik];
}

Vector2i InputManager::GetMousePos() {
	return mousePos_;
}

Vector2i InputManager::GetPreMousePos() {
	return preMousePos_;
}

bool InputManager::IsTriggerMouse(int buttonNumber) {
	return Novice::IsTriggerMouse(buttonNumber);
}

bool InputManager::IsPressMouse(int buttonNumber) {
	return Novice::IsPressMouse(buttonNumber);
}

int InputManager::GetWheel() {
	return Novice::GetWheel();
}
