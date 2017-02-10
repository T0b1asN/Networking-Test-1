#pragma once
#include "SFML.h"
#include "Defines.h"

#include <iostream>
#include <string>

class Server
{
private:
	sf::TcpListener listener;

	unsigned int port;
	unsigned int max_Clients;

	sf::IpAddress ip;
	std::string name;

	sf::TcpSocket other;

	bool run = true;

	sf::Packet sendData;
	sf::Packet receiveData;

	std::string lastMsg;

public:
	Server(std::string pName, unsigned int pPort = 53000, unsigned int pMax_Clients = 10, sf::IpAddress adress = sf::IpAddress::getLocalAddress());
	~Server();

	std::string getInfo();
	std::string getName() { return name; }

	bool newMsg;

	std::string getLastMsg() { return lastMsg; }

	void setup();
	void connectToClient();

	bool isRun() { return run; }

	void SendString(std::string msg);

	void Update();
};

