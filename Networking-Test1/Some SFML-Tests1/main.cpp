#include "SFML.h"
#include "Defines.h"

#include "Server.h"
#include "Client.h"
#include "curr.h"

#include "StartMenu.h"

#include <iostream>
#include <string>
#include <ctime>

void RunServer();
void RunClient();
std::string getCurrTime();

void GraphicsSetup(unsigned int width, unsigned int height);

sf::RenderWindow win;
sf::Font mainFont;

int main()
{
	//TODO: try winsock
	GraphicsSetup(500U, 150U);
	
	StartMenu stMen;

	StartMenu::Result stMenRes = stMen.open();
	//std::cout << "Version " << VERSION << std::endl;
	//std::cout << "Time: " << getCurrTime() << std::endl;
	//char in;
	//std::cout << "[s] for Server. [c] for Client." << std::endl;
	//std::cin >> in;

	switch (stMenRes)
	{
	case StartMenu::Server:
		GraphicsSetup(1000U, 750U);
		RunServer();

		break;
	case StartMenu::Client:
		GraphicsSetup(1000U, 750U);
		RunClient();

		break;
	case StartMenu::Close:
		return 0;
		break;
	}

	return 0;
}

void GraphicsSetup(unsigned int width, unsigned int height)
{
	std::string vers = VERSION;
	win.create(sf::VideoMode(width, height), "SFML-Networkingtest, Version " + vers, sf::Style::Close);
	win.setFramerateLimit(60);

	std::string fontName = FONT_NORM;
	mainFont.loadFromFile("res\\fonts\\" + fontName);

	sf::sleep(sf::milliseconds(50));
}

void RunServer()
{
	Server server("Test", false, 53000, 1);
	server.setup();
	server.connectToClient();
	server.Run();
}

void RunClient()
{
	Client client("Client 1", false);
	client.setup();
	client.Run();
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


//Returns for current stuff
sf::RenderWindow& cr::currWin() { return win; }
unsigned int cr::winWidth() { return win.getSize().x; }
unsigned int cr::winHeight() { return win.getSize().y; }
sf::Font& cr::currFont() { return mainFont; }
