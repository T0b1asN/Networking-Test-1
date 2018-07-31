#pragma once
#include "SFML.h"
#include "Defines.h"

#include "TextBox.h"
#include "CheckBox.h"
#include "OwnButton.h"
#include "NamePrompt.h"

#include "util.h"

#include <iostream>
#include <string>

#include <vector>

#include <Windows.h>

class Client
{
private:
	unsigned int port;
	std::string name;
	sf::IpAddress ip;

	sf::TcpSocket socket;

	sf::Packet sendData;
	sf::Packet receiveData;

	sf::String lastMsg;

	bool connected = true;
	bool block;

	unsigned int maxMsgs = 15U;

	std::vector<sf::String> msgs;

	bool muted;
	CheckBox muteBox;

	OwnButton sendButton;

public:
	Client(bool pBlock, int pPort = 53000, sf::IpAddress address = sf::IpAddress::getPublicAddress());
	~Client();

	bool isConnected() { return connected; }

	std::string getName() { return name; }
	void SendString(sf::String msg);
	sf::String getLastMsg() { return lastMsg; }

	bool newMsg;

	//returns errorCode
	//	0 = OK
	//	1 = Could not connect
	//	2 = Closed name prompt
	//	3 = Reached code that is unreachable
	int setup();

	unsigned int getPort() { return socket.getRemotePort(); }

	void OnServerDisconnect();

private:
	sf::Text nameText;
	TextBox textBox;
	sf::Text msgText;

	void Draw();

	void Enter();

	void initGraphics();
public:
	//Displays message in messages feed (no name, etc.)
	void DisplayMessage(std::string message);

	//General
private:
	bool run = true;

public:
	void Update();
	void Run();

private:
#pragma region Callbacks
	void initCallbacks();
#pragma endregion

public:
#pragma region Callbacks
	// Callback stuff
	const std::string callback_id = "client";

	void leftMCallback(int x, int y);
	// handle for leftMouseCallback
	input::mouseCallback lMCb =
		std::bind(
			&Client::leftMCallback, this,
			std::placeholders::_1,
			std::placeholders::_2
		);

	void closeCallback();
	// handle for closeCallback
	input::closeCallback cCb =
		std::bind(&Client::closeCallback, this);

	void textEnteredCallback(sf::Event::TextEvent text);
	// handle for textEnteredCallback
	input::textEnteredCallback tECb =
		std::bind(
			&Client::textEnteredCallback, this,
			std::placeholders::_1
		);

#pragma endregion


};

