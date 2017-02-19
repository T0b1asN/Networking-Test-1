#pragma once
#include "SFML.h"
#include "Defines.h"

#include <iostream>
#include <string>

#include "TextBox.h"

#include <vector>

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
	Client(std::string pName, bool pBlock, int pPort = 53000, sf::IpAddress address = sf::IpAddress::getPublicAddress());
	~Client();

	bool isConnected() { return connected; }

	std::string getName() { return name; }
	void SendString(sf::String msg);
	sf::String getLastMsg() { return lastMsg; }

	bool newMsg;

	void setup();

	void receive();

	unsigned int getPort() { return socket.getRemotePort(); }

private:
	sf::Text nameText;
	TextBox textBox;
	sf::Text msgText;

	void Draw();

	void Enter();

	void initGraphics();
public:

	//General
public:
	void Update();
	void Run();
};

