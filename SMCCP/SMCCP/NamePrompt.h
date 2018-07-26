#pragma once
#include "TextBox.h"
#include "OwnButton.h"
#include "SFML.h"

#include "InputHandler.h"

class NamePrompt
{
private:
	TextBox nameBox;
	std::string name;

	OwnButton okButton;

	sf::RenderWindow prompt;

	int returnVal = -1;

	void initCallbacks();

public:
	NamePrompt();
	~NamePrompt();

	const std::string callback_id = "namePrompt";

	//returns errorCode
	//	0 = OK
	//	1 = Closed
	//	2 = reached end of function (bad)
	int run();

	std::string getName() { return name; }

	void leftMouseDown(int x, int y);
	void textEntered(sf::Event::TextEvent text);
	void close();

	void display();
	void nextWindow();
};

