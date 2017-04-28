#pragma once
#include "SFML.h"
#include "Defines.h"

#include "TextBox.h"

#include "AudioUtil.h"
#include "NamePrompt.h"
#include "LogUtil.h"

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
public:
	void Update();
	void Run();
};

