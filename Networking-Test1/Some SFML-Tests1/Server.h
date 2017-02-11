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

	sf::String lastMsg;
	
	bool block;
public:
	Server(std::string pName, bool pBlock, unsigned int pPort = 53000, unsigned int pMax_Clients = 10, sf::IpAddress adress = sf::IpAddress::getLocalAddress());
	~Server();

	std::string getInfo();
	std::string getName() { return name; }

	bool newMsg;

	sf::String getLastMsg() { return lastMsg; }

	void setup();
	void connectToClient();

	bool isRun() { return run; }

	void SendString(sf::String msg);

	void Update();
};

