#pragma once
#include "curr.h"
#include "SFML.h"
#include <functional>

namespace input
{
	void addLeftMouseCallback(std::function<void(int, int)> cb, std::string name);
	void deleteLMouseCallback(std::string name);
	std::map<std::string, std::function<void(int, int)>> getLMouseCallbacks();
	std::function<void(int, int)> getLMouseCallback(std::string name);

	void addCloseCallback(std::function<void()> cb, std::string name);
	void deleteCloseCallback(std::string name);
	std::map<std::string, std::function<void()>> getCloseCallbacks();
	std::function<void()> getCloseCallback(std::string name);

	void addTextEnteredCallback(std::function<void(sf::Event::TextEvent)> cb, std::string name);
	void deleteTextEnteredCallback(std::string name);
	std::map<std::string, std::function<void(sf::Event::TextEvent)>> getTextEnteredCallbacks();
	std::function<void(sf::Event::TextEvent)> getTextEnteredCallback(std::string name);

	void setFocus(sf::RenderWindow* newFocus);

	// Don't call by yourself
	void cleanCallbacks();

	void handleInput();
}
