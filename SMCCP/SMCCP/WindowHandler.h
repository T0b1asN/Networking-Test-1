#pragma once
#include "curr.h"
#include <vector>
#include <string>
#include <map>

namespace windows
{
	// a class that gives new windows if you ask
	//TODO get a better name

	/*
		This class also needs to handle the events for the events of a single window

		Start with:
			every window only has one class, that gets notified for that windows events
	
		Needs to iterate through all windows and check for it's events

		Needs to be able to control focus loss
	*/


	struct WindowEntry
	{
		WindowEntry(std::string key, sf::RenderWindow* window)
		{
			this->key = key;
			this->window = window;
		}

		std::string key;
		sf::RenderWindow* window;
	};
	sf::RenderWindow* getNew(const std::string& id);

	bool destroyWindow(WindowEntry& win);
	bool destroyWindow(const std::string& key);
}
