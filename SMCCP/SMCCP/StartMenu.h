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

#include "InputHandler.h"

class StartMenu
{
public:
	enum Result
	{
		None = -2,
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

	Result returnVal = Result::None;

	void initCallbacks();

public:
	StartMenu();
	~StartMenu();

	const std::string callback_id = "startMenu";

	Result open();

	sf::IpAddress getIp() { return adress; }
	unsigned int getPort() { return port; }

	void leftMouseDown(int x, int y);
	void textEntered(sf::Event::TextEvent text);
	void close();

	void display();
	void nextWindow();
};

