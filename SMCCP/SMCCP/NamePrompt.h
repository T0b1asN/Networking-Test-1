#pragma once
#include "BaseUIWindow.h"

class NamePrompt : public BaseUIWindow
{
private:
	TextBox nameBox;
	std::string name;

	OwnButton okButton;

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

