#pragma once
#include "SFML\Audio.hpp"
#include "LogUtil.h"

#include <map>
#include <VECTOR>
#include <string>

#define SOUND_PATH "res\\sounds\\"

namespace snd
{
	extern std::map<std::string, sf::SoundBuffer> sounds;

	extern std::vector<sf::Sound> players;
	extern int channels;
	extern int currChannel;

	bool LoadAllSounds();

	void playSound(std::string name);
}
