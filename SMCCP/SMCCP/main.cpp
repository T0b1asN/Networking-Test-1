#define _CRT_SECURE_NO_WARNINGS

#include "SFML.h"
#include "Defines.h"

#include "Server.h"
#include "Client.h"
#include "curr.h"

#include "StartMenu.h"
#include "NamePrompt.h"

#include <iostream>
#include <string>
#include <ctime>

#include "AudioUtil.h"
#include "NetworkHelpers.h"

void RunServer(std::string name, int port = 1234);
void RunClient(sf::IpAddress adress, unsigned int port = 1234);
std::string getCurrTime();

sf::Image icon;

void GraphicsSetup(unsigned int width, unsigned int height);

sf::RenderWindow win;
sf::Font mainFont;

int main()
{
	if (own_log::CreateLog() == 0)
	{
		own_log::pushMsgToCommandIfDebug("Created log file");
		own_log::AppendToLog("Created log file");
	}

	if (!snd::LoadAllSounds())
		own_log::pushMsgToCommandIfDebug("Couldn't load all sounds");
	
	GraphicsSetup(500U, 200U);
	
#ifndef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // !NDEBUG

	StartMenu stMen;
	
	StartMenu::Result stMenRes = stMen.open();
	sf::IpAddress enteredIp = stMen.getIp();
	unsigned int port = stMen.getPort();
	own_log::pushMsgToCommandIfDebug("Port: " + std::to_string(port) + "Ip: " + enteredIp.toString());

	switch (stMenRes)
	{
	case StartMenu::Server:
	{
		win.close();

		NamePrompt np;
		if (np.run() == 1)
			return 0;
		std::string name = np.getName();

		GraphicsSetup(1000U, 750U);
		RunServer(name, port);
		break;
	}
	case StartMenu::Client:
		GraphicsSetup(1000U, 750U);
		RunClient(enteredIp, port);

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
	win.create(sf::VideoMode(width, height), "SMCCP " + vers, sf::Style::Close);
	win.setFramerateLimit(60);

	if (icon.loadFromFile("res\\AppIcon.png"))
		win.setIcon(626, 626, icon.getPixelsPtr());
	else
		own_log::AppendToLog("Icon could not be loaded!");
	std::string fontName = FONT_NORM;
	mainFont.loadFromFile("res\\fonts\\" + fontName);

	sf::sleep(sf::milliseconds(50));
}

void RunServer(std::string name, int port)
{
	Server server(name, false, port, 10);
	int setupCode = server.setup();
	if (setupCode != 0)
	{
		own_log::AppendToLog("Error " + std::to_string(setupCode) + " while setting up the server");
		return;
	}
	server.connectToClient();
	server.Run();
}

void RunClient(sf::IpAddress adress, unsigned int port)
{
	Client client(false, port, adress);
	if(client.setup() == 2)
		return;
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
