#pragma once
#include "SFML.h"
#include "Defines.h"

#include <iostream>
#include <string>

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

public:
	Client(std::string pName, bool pBlock, int pPort = 53000, sf::IpAddress address = sf::IpAddress::getLocalAddress());
	~Client();

	bool isConnected() { return connected; }

	std::string getName() { return name; }
	void SendString(sf::String msg);
	sf::String getLastMsg() { return lastMsg; }

	bool newMsg;

	void setup();
	void Update();

	void receive();

	unsigned int getPort() { return socket.getRemotePort(); }
};

