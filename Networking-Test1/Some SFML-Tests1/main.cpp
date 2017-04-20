#define _CRT_SECURE_NO_WARNINGS

#include "SFML.h"
#include "Defines.h"

#include "Server.h"
#include "Client.h"
#include "curr.h"

#include "StartMenu.h"

#include <iostream>
#include <string>
#include <ctime>

void RunServer(std::string name);
void RunClient(std::string name, sf::IpAddress adress);
std::string getCurrTime();

void GraphicsSetup(unsigned int width, unsigned int height);

sf::RenderWindow win;
sf::Font mainFont;

int main()
{
	GraphicsSetup(500U, 250U);
	
	StartMenu stMen;

	StartMenu::Result stMenRes = stMen.open();
	std::string enteredName = stMen.getName();
	sf::IpAddress enteredIp = stMen.getIp();
	std::cout << "Name: " << enteredName << " | Ip: " << enteredIp.toString() << std::endl;

	switch (stMenRes)
	{
	case StartMenu::Server:
		GraphicsSetup(1000U, 750U);
		RunServer(enteredName);

		break;
	case StartMenu::Client:
		GraphicsSetup(1000U, 750U);
		RunClient(enteredName, enteredIp);

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

void RunServer(std::string name)
{
	Server server(name, false, 1234, 10);
	int setupCode = server.setup();
	if (setupCode != 0)
	{
		std::cerr << "Error " << setupCode << " while setting up the server" << std::endl;
		return;
	}
	server.connectToClient();
	server.Run();
}

void RunClient(std::string name, sf::IpAddress adress)
{
	Client client(name, false, 1234, adress);
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
