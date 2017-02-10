#include "SFML.h"
#include "Defines.h"

#include "Server.h"
#include "Client.h"

#include <iostream>
#include <string>
#include <ctime>

void RunServer();
void RunClient();
std::string getCurrTime();

int main()
{
	char in;
	std::cout << "[s] for Server. [c] for Client." << std::endl;
	std::cin >> in;

	switch (in)
	{
	case 's':
	case 'S':
		RunServer();

		break;
	case 'c':
	case 'C':
		RunClient();

		break;
	}

	std::system("PAUSE");
	return 0;
}

void RunServer()
{
	Server server("Test", 53000, 1);
	std::cout << server.getInfo() << std::endl;
	server.setup();
	server.connectToClient();

	while (server.isRun())
	{
		server.SendString("Hi. You are connected to " + server.getName());
		server.Update();
		if (server.newMsg)
			std::cout << server.getLastMsg() << std::endl;
		sf::sleep(sf::seconds(5));
	}
}

void RunClient()
{
	Client client("Client 1");
	std::cout << client.getName() << std::endl;

	while (client.isConnected())
	{
		client.SendString("HI: " + getCurrTime());
		sf::sleep(sf::seconds(5));
	}
}

std::string getCurrTime()
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	//cout << (now->tm_year + 1900) << '-'
	//	<< (now->tm_mon + 1) << '-'
	//	<< now->tm_mday
	//	<< endl;
	std::string out;
	out += std::to_string(now->tm_mday) + "/" + std::to_string(now->tm_mon + 1) + "/" + std::to_string(now->tm_year + 1900);
	return out;
}
