#include "AudioUtil.h"

std::map<std::string, sf::SoundBuffer> snd::sounds;

std::vector<sf::Sound> snd::players;
int snd::channels = 8;
int snd::currChannel = 0;

bool snd::LoadAllSounds()
{
	own_log::AppendToLogWOTime("\n**********************************\nLoading audio files");
	bool no_error = true;

	sf::SoundBuffer bufToCopy;
	//Set up players
	for (int i = 0; i < snd::channels; i++)
		snd::players.push_back(sf::Sound());

	std::string path = SOUND_PATH;

	//loading msg_incmg_01.wav in to map
	if (!bufToCopy.loadFromFile(path + "msg_incmg_01.wav"))
	{
		no_error = false;
		own_log::AppendToLog("Error loading: msg_incmg_01.wav");
	}
	sounds.emplace("incoming_01", sf::SoundBuffer(bufToCopy));

	//loading error_01.wav in to map
	if (!bufToCopy.loadFromFile(path + "error_01.wav"))
	{
		no_error = false;
		own_log::AppendToLog("Error loading: error_01.wav");
	}
	sounds.emplace("error_01", sf::SoundBuffer(bufToCopy));

	//loading msg_snd_01.wav in to map
	if (!bufToCopy.loadFromFile(path + "msg_snd_01.wav"))
	{
		no_error = false;
		own_log::AppendToLog("Error loading: msg_snd_01.wav");
	}
	sounds.emplace("send_01", sf::SoundBuffer(bufToCopy));

	std::string result = (no_error ? "true" : "false");
	own_log::AppendToLog("Loaded " + std::to_string(sounds.size()) + " audiofiles - without error: " + result);
	own_log::AppendToLogWOTime("**********************************");

	return no_error;
}

void snd::playSound(std::string name)
{
	//play sound on current Channel
	snd::players.at(snd::currChannel).setBuffer(sounds.at(name));
	snd::players.at(snd::currChannel).play();

	//iterate the channel
	currChannel++;
	if (currChannel == channels)
		currChannel = 0;
}