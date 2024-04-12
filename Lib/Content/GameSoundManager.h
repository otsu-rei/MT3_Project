#pragma once
// Novice ---
#include <Novice.h>

// std:: ---
#include <map>
#include <string>
#include <list>

struct SoundElement;
struct SoundList;

class GameSoundManager final {
public: // default method //

	// constructor & destructor
	GameSoundManager();
	~GameSoundManager();

	static void Load();

private: // menber object //

	static std::map<std::string, SoundElement> sounds_;
	static std::list<SoundList> voiceHandles_;

public: // user method //

	static void PlayAudio();

	static void SetAudio(std::string soundName, bool roop, bool duplication = false);

	static void StopAudio();
	static void StopAudio(std::string soundName);

	

public: // accessor //
};

struct SoundElement {
	int handle;

	std::string filePath;
	float defaultVolume;
};

struct SoundList {
	int voiceHandle;

	std::string soundName;
	bool loop; // t -> one more, f -> remove
};