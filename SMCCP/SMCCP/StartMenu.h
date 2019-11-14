#pragma once
#include "SFML.h"
#include "curr.h"
#include "Defines.h"

#include "TextBox.h"
#include "CheckBox.h"
#include "OwnButton.h"

#include <iostream>
#include <string>

#include "InputHandler.h"

#include "InputCallbackHandler.h"

class StartMenu : public InputCallbackHandler
{
public:
	enum class Result
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

	static const std::string serverButton_id;
	static const std::string clientButton_id;

public:
	StartMenu();
	~StartMenu();

	Result open();

	sf::IpAddress getIp() { return adress; }
	unsigned int getPort() { return port; }

	virtual void LeftMCallback(int x, int y);
	virtual void TextEnteredCallback(sf::Event::TextEvent text);
	virtual void CloseCallback();
	virtual void LostFocusCallback();

	void display();
	void nextWindow();
};

