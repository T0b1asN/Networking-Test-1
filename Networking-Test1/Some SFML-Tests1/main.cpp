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
	std::cout << "Version " << VERSION << std::endl;
	std::cout << "Time: " << getCurrTime() << std::endl;
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
	Server server("Test", false, 53000, 1);
	server.setup();
	server.connectToClient();
	server.SendString("Hi");

	while (server.isRun())
	{
		
	}
}

void RunClient()
{
	Client client("Client 1", false);
	client.setup();
	while (client.isConnected())
	{
		client.Update();
	}
}

std::string getCurrTime()
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	std::string out;
	out += std::to_string(now->tm_mday) + 
		"/" + std::to_string(now->tm_mon + 1) + 
		"/" + std::to_string(now->tm_year + 1900) + 
		" - " + std::to_string(now->tm_hour) + 
		":" + std::to_string(now->tm_min);
	return out;
}
