#pragma once
#include "TextBox.h"
#include "OwnButton.h"
#include "SFML.h"

#include "InputHandler.h"

#include <Windows.h>

class NamePrompt
{
private:
	TextBox nameBox;
	std::string name;

	OwnButton okButton;

	sf::RenderWindow prompt;

	int returnVal = -1;

	void initCallbacks();
	void cleanCallbacks();

	bool loseFocus;

public:
	NamePrompt(bool dontLoseFocus = true);
	~NamePrompt();

	const std::string callback_id = "namePrompt";

	//returns errorCode
	//	0 = OK
	//	1 = Closed
	//	2 = reached end of function (bad)
	int run();

	std::string getName() { return name; }


	void display();
	void nextWindow();

	// Callback stuff
	void leftMouseDown(int x, int y);
	// handle for leftMouseCallback
	input::mouseCallback lMCb =
		std::bind(
			&NamePrompt::leftMouseDown, this,
			std::placeholders::_1,
			std::placeholders::_2
		);
	
	void close();
	// handle for closeCallback
	input::closeCallback cCb =
		std::bind(&NamePrompt::close, this);
	
	void textEntered(sf::Event::TextEvent text);
	// handle for textEnteredCallback
	input::textEnteredCallback tECb =
		std::bind(
			&NamePrompt::textEntered, this,
			std::placeholders::_1
		);

	void lostFocus();
	input::lostFocusCallback lFCb =
		std::bind(&NamePrompt::lostFocus, this);
};

