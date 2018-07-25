#pragma once
#include "curr.h"
#include "SFML.h"
#include <functional>

namespace input
{
	typedef std::function<void(int, int)> mouseCallback;
	void addLeftMouseCallback(mouseCallback cb, std::string name);
	void deleteLMouseCallback(std::string name);
	std::map<std::string, mouseCallback> getLMouseCallbacks();
	mouseCallback getLMouseCallback(std::string name);

	typedef std::function<void()> closeCallback;
	void addCloseCallback(closeCallback cb, std::string name);
	void deleteCloseCallback(std::string name);
	std::map<std::string, closeCallback> getCloseCallbacks();
	closeCallback getCloseCallback(std::string name);

	typedef std::function<void(sf::Event::TextEvent)> textEnteredCallback;
	void addTextEnteredCallback(textEnteredCallback cb, std::string name);
	void deleteTextEnteredCallback(std::string name);
	std::map<std::string, textEnteredCallback> getTextEnteredCallbacks();
	textEnteredCallback getTextEnteredCallback(std::string name);

	void setFocus(sf::RenderWindow* newFocus);

	// Don't call by yourself
	void cleanCallbacks();

	void handleInput();
}
