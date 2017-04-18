#pragma once
#include "SFML.h"
#include "Defines.h"
#include "curr.h"

#include "TextBox.h"

#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

#include <memory>

class Server
{
	//Networking stuff
private:
	sf::TcpListener listener;

	unsigned int port;
	unsigned int max_Clients;

	sf::IpAddress ip;
	std::string name;

	sf::SocketSelector selector;
	std::vector<std::unique_ptr<sf::TcpSocket>> sockets;

	bool run = true;

	sf::Packet sendData;
	sf::Packet receiveData;

	sf::String lastMsg;

	unsigned int maxMsgs = 15;
	std::vector<sf::String> msgs;
	
	bool block;

	void pushNewSocket();

	int socketsConnected = 0;
public:
	Server(std::string pName, bool pBlock, unsigned int pPort = 53000, unsigned int pMax_Clients = 10, sf::IpAddress adress = sf::IpAddress::getPublicAddress());
	~Server();

	std::string getInfo();
	std::string getName() { return name; }

	bool newMsg;

	sf::String getLastMsg() { return lastMsg; }

	void setup();
	void connectToClient();

	bool isRun() { return run; }

	void SendString(sf::String msg);
	void SendString(sf::String msg, int exclude);

	//Graphics Stuff
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

