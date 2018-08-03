#pragma once
#include "curr.h"
#include "SFML.h"
#include <functional>

namespace input
{
	//TODO look into callbacks from other sources
	typedef const std::string& id;

	typedef std::function<void(int, int)> mouseCallback;
	void addLeftMouseCallback(mouseCallback cb, id name);
	void deleteLMouseCallback(id name);
	std::map<std::string, mouseCallback> getLMouseCallbacks();
	mouseCallback getLMouseCallback(id name);

	typedef std::function<void()> closeCallback;
	void addCloseCallback(closeCallback cb, id name);
	void deleteCloseCallback(id name);
	std::map<std::string, closeCallback> getCloseCallbacks();
	closeCallback getCloseCallback(id name);

	typedef std::function<void(sf::Event::TextEvent)> textEnteredCallback;
	void addTextEnteredCallback(textEnteredCallback cb, id name);
	void deleteTextEnteredCallback(id name);
	std::map<std::string, textEnteredCallback> getTextEnteredCallbacks();
	textEnteredCallback getTextEnteredCallback(id name);

	typedef std::function<void()> lostFocusCallback;
	void addLostFocusCallback(lostFocusCallback cb, id name);
	void deleteLostFocusCallback(id name);
	std::map<std::string, lostFocusCallback> getLostFocusCallbacks();
	lostFocusCallback getLostFocusCallback(id name);

	void setFocus(sf::RenderWindow* newFocus);

	// Don't call by yourself
	void cleanCallbacks();

	void handleInput();
}
