#include "WindowHandler.h"

namespace windows
{
	namespace
	{
		std::map<std::string, sf::RenderWindow*> windows;
	}

	sf::RenderWindow* getNew(const std::string& id)
	{
		if (windows.count(id) > 0)
			return windows[id];
		return &sf::RenderWindow();
	}
	
	WindowEntry* getNew()
	{
		// TODO change
		std::string s = str::createRandom(KEY_LENGTH);
		while (windows.count(s) > 0)
			s = str::createRandom(KEY_LENGTH);
		return new WindowEntry(s, getNew(s));
	}

	bool destroyWindow(WindowEntry& win)
	{
		return false;
	}

	bool destroyWindow(const std::string& key)
	{
		return false;
	}
}