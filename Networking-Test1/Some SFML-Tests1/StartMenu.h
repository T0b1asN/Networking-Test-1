#pragma once
#include "SFML.h"
#include "curr.h"
#include "OwnButton.h"
#include "Defines.h"

#include <iostream>
#include <string>

class StartMenu
{
public:
	enum Result
	{
		Close = -1,
		Default = 0,
		Server = 1,
		Client = 2
	};

private:
	OwnButton serverButton;
	OwnButton clientButton;

public:
	StartMenu();
	~StartMenu();

	Result open();
	void display();
};

