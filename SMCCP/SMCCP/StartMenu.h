#pragma once
#include "SFML.h"
#include "curr.h"
#include "Defines.h"
#include "NetworkHelpers.h"

#include "OwnButton.h"
#include "TextBox.h"
#include "CheckBox.h"

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

	TextBox ipBox;
	sf::IpAddress adress;

	TextBox portBox;
	unsigned int port;

public:
	StartMenu();
	~StartMenu();

	Result open();

	sf::IpAddress getIp() { return adress; }
	unsigned int getPort() { return port; }

	void display();
};

