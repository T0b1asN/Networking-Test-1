#include "Client.h"



Client::Client(std::string pName, int pPort, sf::IpAddress address)
{
	port = pPort;
	name = pName;
	ip = address;
}


Client::~Client()
{

}

void Client::SendString(std::string msg)
{
	sendData.clear();
	sendData << msg;
	socket.send(sendData);
}

void Client::setup()
{
	if (socket.connect(ip, port, sf::seconds(2.f)) != sf::Socket::Done)
	{
		//TODO: Client-Error Could not Connect
	}

	SendString("Hi");
}
