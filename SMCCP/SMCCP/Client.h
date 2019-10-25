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

#include "RSA.h"
#include "Protocol.h"

class Client
{
public:
#pragma region Con-/Destructor
	Client(bool pBlock, int pPort = 53000, sf::IpAddress address = sf::IpAddress::getPublicAddress());
	~Client();
#pragma endregion

private:
#pragma region Networking
	unsigned int port;
	std::string name;
	sf::IpAddress ip;

	sf::TcpSocket socket;

	sf::Packet sendData;
	sf::Packet receiveData;

	bool connected = true;
	bool block;

	void onServerDisconnect();

	std::string lastMsg;
	unsigned int maxMsgs = 15U;

	std::vector<sf::String> msgs;
#pragma endregion

public:
#pragma region Networking
	__declspec(deprecated) void SendString(sf::String msg);

	//set up the networking
	//returns errorCode
	//	0 = OK
	//	1 = Could not connect
	//	2 = Closed name prompt
	//	3 = Reached code that is unreachable
	int Setup();

	bool isConnected() { return connected; }
	std::string getName() { return name; }
	sf::String getLastMsg() { return lastMsg; }
	unsigned int getPort() { return socket.getRemotePort(); }

	void Send(std::string msg, bool tagIncluded = false, bool encrypt = true);
#pragma endregion

private:
#pragma region Graphics
	sf::Text nameText;
	TextBox textBox;
	sf::Text msgText;

	CheckBox muteBox;

	OwnButton sendButton;

	void draw();
	void onEnter();
	void initGraphics();
#pragma endregion

public:
#pragma region Graphics
	//Displays message in messages feed (no name, etc.)
	void DisplayMessage(std::string message);
#pragma endregion

	//General
private:
#pragma region General
	bool running = true;
	bool muted;

	void update();
#pragma endregion

public:
#pragma region General
	void Run();
#pragma endregion

private:
#pragma region RSA
	RSA::Key key;
	RSA::PublicKey serverKey;

	//tries generating a key (filling variable key)
	//returns true if succesful
	bool GenerateKey(int max_errors = 5);
#pragma endregion

public:
#pragma region RSA

#pragma endregion

private:
#pragma region Callbacks
	void initCallbacks();
	void cleanCallbacks();

	static const std::string sendButton_id;
#pragma endregion

public:
#pragma region Callbacks
	// Callback stuff
	const std::string callback_id = "client";

	void LeftMCallback(int x, int y);
	// handle for leftMouseCallback
	input::mouseCallback lMCb =
		std::bind(
			&Client::LeftMCallback, this,
			std::placeholders::_1,
			std::placeholders::_2
		);

	void CloseCallback();
	// handle for closeCallback
	input::closeCallback cCb =
		std::bind(&Client::CloseCallback, this);

	void TextEnteredCallback(sf::Event::TextEvent text);
	// handle for textEnteredCallback
	input::textEnteredCallback tECb =
		std::bind(
			&Client::TextEnteredCallback, this,
			std::placeholders::_1
		);

	void buttonCallback(std::string id);
	OwnButton::buttonCallback bCallback =
		std::bind(&Client::buttonCallback, this,
			std::placeholders::_1);
#pragma endregion
};

