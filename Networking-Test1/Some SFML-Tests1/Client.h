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

	bool connected = true;

public:
	Client(std::string pName, int pPort = 53000, sf::IpAddress address = sf::IpAddress::getLocalAddress());
	~Client();

	bool isConnected() { return connected; }

	std::string getName() { return name; }
	void SendString(std::string msg);

	void setup();
};

