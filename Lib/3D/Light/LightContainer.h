#pragma once

// std:: ---
#include <string>
#include <map>

// Lib > 3D ---
#include "Light.h"
#include "Camera3D.h"

class LightContainer {
public: // public method //

	LightContainer();
	~LightContainer();

	void SetContainer(
		std::string name, const Light* light);

	void ClearContainer();

	void RemoveLight(
		std::string name);

	void DrawLight(
		const Camera3D& camera); // pre

private: // menber object //

	std::map<std::string, const Light*> container_;

public: // accessor //

	std::map<std::string, const Light*> GetLightContainer() const { return container_; }

};

