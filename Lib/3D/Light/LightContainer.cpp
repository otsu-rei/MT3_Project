#include "LightContainer.h"

//-----------------------------------------------------------------------------------------
// constructor & destructor
//-----------------------------------------------------------------------------------------
LightContainer::LightContainer() {
}

LightContainer::~LightContainer() {
	ClearContainer();
}

//-----------------------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------------------
void LightContainer::SetContainer(
	std::string name, const Light* light) {

	container_.insert(std::make_pair(name, light));
}

void LightContainer::ClearContainer() {
	container_.clear();
}

void LightContainer::RemoveLight(
	std::string name) {

	container_.erase(name);
}

void LightContainer::DrawLight(
	const Camera3D& camera) {

	for (const auto& light : container_) {
		light.second->Draw(camera);
	}
}
