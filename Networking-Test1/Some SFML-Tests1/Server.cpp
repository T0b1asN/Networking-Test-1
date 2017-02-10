#include "Server.h"

Server::Server(std::string pName, unsigned int pPort, unsigned int pMax_Clients, sf::IpAddress address)
{
	port = pPort;
	max_Clients = pMax_Clients;
	ip = address;
	name = pName;
}

Server::~Server()
{

}

std::string Server::getInfo()
{
	std::string out;
	out += "Name: " + name 
		+ "; Port: " + std::to_string(port) 
		+ "; Max Clients: " + std::to_string(max_Clients) 
		+ "; Ip: " + ip.toString();
	return out;
}

void Server::setup()
{
	if (listener.listen(port) != sf::Socket::Status::Done)
	{
		//TODO: Server-Error Could not set up server
	}
	std::cout << "Connected... Port: " << port << std::endl;
}

void Server::connectToClient()
{
	if (listener.accept(other) != sf::Socket::Status::Done)
	{
		//TODO: Server-Error Could not connect to Client
	}
	other.setBlocking(false);
}

void Server::SendString(std::string msg)
{
	sendData.clear();
	sendData << msg;
	//other.send(sendData);
	other.send(sendData);
}

void Server::Update()
{
	newMsg = false;
	lastMsg = "";
	receiveData.clear();

	if (other.receive(receiveData) != sf::Socket::Done)
	{
		//No data
	}
	else
	{
		receiveData >> lastMsg;
		newMsg = true;
	}
}
