#pragma once
#include "SFML.h"
#include "Defines.h"
#include "util.h"

#include <iostream>
#include <string>
#include <vector>

#include <map>

#include <Windows.h>

#include <memory>

//This Server has no RSA encryption builtin

class BaseServer
{
protected:
	BaseServer(bool pBlock, unsigned int pPort = DEFAULT_PORT, unsigned int pMaxClients = 10);
	~BaseServer();

private:
	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector<std::unique_ptr<sf::Socket>> sockets;

	const unsigned int port;
	const unsigned int maxClients;

	sf::Packet sendData;
	sf::Packet receiveData;

	const bool block;

protected:
	const unsigned int msgLogMaxLength;
	std::vector<sf::String> msgLog;

	void pushNewSocket();
	int socketsConnected = 0;
	std::vector<sf::String> names;

};

