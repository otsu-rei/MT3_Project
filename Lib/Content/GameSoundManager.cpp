#include "GameSoundManager.h"

std::map<std::string, SoundElement> GameSoundManager::sounds_;
std::list<SoundList> GameSoundManager::voiceHandles_;

GameSoundManager::GameSoundManager() {
	Load();
}

GameSoundManager::~GameSoundManager() {
	sounds_.clear();
	voiceHandles_.clear();
}

void GameSoundManager::Load() {
	sounds_["mokugyo"] = { NULL, "./NoviceResources/mokugyo.wav", 0.5f };
	sounds_["fanfare"] = { NULL, "./NoviceResources/fanfare.wav", 0.5f };

	for (auto it = sounds_.begin(); it != sounds_.end(); it++) {
		it->second.handle = Novice::LoadAudio(it->second.filePath.c_str());
	}
}

void GameSoundManager::PlayAudio() {
	if (voiceHandles_.empty()) { return; }

	for (auto it = voiceHandles_.begin(); it != voiceHandles_.end();) {
		if (it->voiceHandle == -1) { // first sounds
			it->voiceHandle = Novice::PlayAudio(sounds_[it->soundName].handle, false, sounds_[it->soundName].defaultVolume);

		} else if (!Novice::IsPlayingAudio(it->voiceHandle)) { // second sounds
			if (it->loop) {
				it->voiceHandle = Novice::PlayAudio(sounds_[it->soundName].handle, true, sounds_[it->soundName].defaultVolume);

			} else {
				it = voiceHandles_.erase(it);
				continue;
			}

		}

		it++;
	}
}

void GameSoundManager::SetAudio(std::string soundName, bool loop, bool duplication) {
	/*voiceHandles_.push_front({ 0, soundName, loop });*/

	if (duplication) {
 		voiceHandles_.push_back({ -1, soundName, loop });

	} else {
		for (const auto& it : voiceHandles_) {
			if (it.soundName == soundName) {
				return;
			}
		}

		voiceHandles_.push_back({ -1, soundName, loop });
	}
}

void GameSoundManager::StopAudio() {
	for (auto it = voiceHandles_.begin(); it != voiceHandles_.end();) {
		Novice::StopAudio(it->voiceHandle);
		it = voiceHandles_.erase(it);
	}
}

void GameSoundManager::StopAudio(std::string soundName) {
	for (auto it = voiceHandles_.begin(); it != voiceHandles_.end();) {
		if (it->soundName == soundName) {
			Novice::StopAudio(it->voiceHandle);
			it = voiceHandles_.erase(it);

		} else {
			it++;
		}
	}
}




