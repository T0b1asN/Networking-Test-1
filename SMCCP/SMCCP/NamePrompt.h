#pragma once
#include "TextBox.h"
#include "OwnButton.h"
#include "SFML.h"

class NamePrompt
{
private:
	TextBox nameBox;
	std::string name;

	OwnButton okButton;

	sf::RenderWindow prompt;

public:
	NamePrompt();
	~NamePrompt();

	//returns errorCode
	//	0 = OK
	//	1 = Closed
	//	2 = reached end of function (bad)
	int run();

	std::string getName() { return name; }

	void display();
};

