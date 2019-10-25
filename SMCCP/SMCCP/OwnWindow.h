#pragma once
#include "SFML.h"
#include "curr.h"
#include "Defines.h"

#include <iostream>
#include <string>

#include "InputHandler.h"

class OwnWindow
{
	//TODO implement abstract class, that handles common callbacks from SFML
public:
	OwnWindow();
	virtual ~OwnWindow() = 0;

protected:
	OwnWindow(std::string cbId);
	std::string callback_id = "client";
	void setCallbackId(std::string newId) { callback_id = newId; }

public:

	std::string getCallbackId() { return callback_id; }
	// Callback stuff
	virtual void LeftMCallback(int x, int y) {};
	// handle for leftMouseCallback
	input::mouseCallback lMCb =
		std::bind(
			&OwnWindow::LeftMCallback, this,
			std::placeholders::_1,
			std::placeholders::_2
		);

	virtual void CloseCallback() {};
	// handle for closeCallback
	input::closeCallback cCb =
		std::bind(&OwnWindow::CloseCallback, this);

	virtual void TextEnteredCallback(sf::Event::TextEvent text) {};
	// handle for textEnteredCallback
	input::textEnteredCallback tECb =
		std::bind(
			&OwnWindow::TextEnteredCallback, this,
			std::placeholders::_1
		);
};

