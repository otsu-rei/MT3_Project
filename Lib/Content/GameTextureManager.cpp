#include "GameTextureManager.h"

std::map<std::string, TextureElement> GameTextureManager::textures_;

/* -----------------------------------------------------------------------------------------
 default method
----------------------------------------------------------------------------------------- */
GameTextureManager::GameTextureManager() {
}

GameTextureManager::~GameTextureManager() {
}

void GameTextureManager::Load() {
	//!< textures_[textureName] = { handle , filePath };
	textures_["white1x1"] = { "./NoviceResources/white1x1.png" };
	textures_["uvCheck"]  = { "./Resources/tori.png" };

	for (auto it = textures_.begin(); it != textures_.end(); it++) {
		it->second.handle = Novice::LoadTexture(it->second.filePath.c_str());
	}
}

/* -----------------------------------------------------------------------------------------
 user method
----------------------------------------------------------------------------------------- */
void GameTextureManager::HotReload() {
	for (auto it = textures_.begin(); it != textures_.end(); it++) {
		Novice::UnloadTexture(it->second.handle);
		it->second.handle = Novice::LoadTexture(it->second.filePath.c_str());
	}
}

void GameTextureManager::TextureManagerImGui() {
	static std::string result = "";
	static std::string output = "";

	ImGui::Begin("TextureManager Window");

	if (ImGui::Button("HotReroad")) {
		HotReload();
		result = "HotReroad() was executed";
	}

	output = "Resurt: " + result;
	ImGui::Text(output.c_str());

	ImGui::End();
}
