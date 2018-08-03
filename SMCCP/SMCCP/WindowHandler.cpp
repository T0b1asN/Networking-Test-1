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

	bool destroyWindow(WindowEntry& win)
	{
		return false;
	}

	bool destroyWindow(const std::string& key)
	{
		return false;
	}
}