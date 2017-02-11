#include "Client.h"



Client::Client(std::string pName, bool pBlock, int pPort, sf::IpAddress address)
{
	port = pPort;
	name = pName;
	ip = address;
	block = pBlock;
}


Client::~Client()
{

}

void Client::SendString(sf::String msg)
{
	sendData.clear();
	sendData << msg;
	socket.send(sendData);
}

void Client::setup()
{
	if (socket.connect(ip, port, sf::seconds(2.f)) != sf::Socket::Done)
	{
		std::cout << "Could not connect" << std::endl;
		return;
		//TODO: Client-Error Could not Connect
	}
	std::cout << "Connected" << std::endl;
	socket.setBlocking(block);
	SendString("Hi");
}

void Client::Update()
{
	newMsg = false;
	lastMsg = "";
	receiveData.clear();

	if (socket.receive(receiveData) == sf::Socket::Done)
	{
		std::cout << "Receiving" << std::endl;
		if (!(receiveData >> lastMsg))
		{
			std::cout << "Error in receiving" << std::endl;
			return;
		}
		if (lastMsg != "")
		{
			lastMsg = "Server: " + lastMsg;
			newMsg = true;
			std::cout << lastMsg.toAnsiString() << std::endl;
		}
	}
}

void Client::receive()
{
	socket.receive(receiveData);
	receiveData >> lastMsg;
	if (lastMsg != "")
		newMsg = true;
}
