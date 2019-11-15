#pragma once
#include "SFML.h"
#include "Defines.h"
#include "util.h"

#include "Protocol.h"

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
	enum class SetupResult
	{
		Done = 0,
		Error = -1
	};
protected:
	BaseServer(bool pBlock, unsigned int pPort = DEFAULT_PORT, unsigned int pMaxClients = 10);
	~BaseServer();

private:
	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector<std::unique_ptr<sf::TcpSocket>> sockets;

	const unsigned int port;
	const unsigned int maxClients;

	sf::Packet sendData;
	sf::Packet receiveData;

	const bool block;

	void disconnectSocket(int index, sf::String reason = "");

	bool running = true;

protected:
	const unsigned int msgLogMaxLength;
	std::vector<sf::String> msgLog;
	void pushNewMsg(sf::String msg);

	void pushNewSocket();
	int socketsConnected = 0;
	std::vector<sf::String> names;

	SetupResult Setup();
	void ConnectClient();

	void Send(sf::String msg, bool alreadyHasToken  = false);
	void Send(sf::String msg, int exclude, bool alreadyHasToken = false);
	void SendSingle(sf::String msg, int socketIndex, bool alreadyHasToken = false);

	void Shutdown(sf::String optMsg = "");

	void Update();
	bool isRunning() { return running; }

	virtual void OnDisconnect() = 0;
	virtual void OnMessage() = 0;

};

