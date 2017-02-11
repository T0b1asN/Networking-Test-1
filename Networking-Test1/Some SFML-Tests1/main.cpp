#include "SFML.h"
#include "Defines.h"

#include "Server.h"
#include "Client.h"
#include "curr.h"

#include "OwnButton.h"

#include <iostream>
#include <string>
#include <ctime>

void RunServer();
void RunClient();
std::string getCurrTime();

void GraphicsSetup();

sf::RenderWindow win;
sf::Font mainFont;

int main()
{
	GraphicsSetup();

	OwnButton test("Test", sf::Vector2f(200.0f, 100.0f), sf::Vector2f(WIDTH / 2.0f, HEIGHT / 2.0f));
	test.SetOrigin(test.GetSize() / 2.0f);

	while (win.isOpen())
	{
		bool click = false;
		sf::Event evnt;
		while (win.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				win.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					click = true;
				}
				break;
			}
		}

		win.clear(sf::Color(100, 100, 100));

		test.Update(click);
		test.display();

		win.display();
	}
	return 0;

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

void GraphicsSetup()
{
	std::string vers = VERSION;
	win.create(sf::VideoMode(WIDTH, HEIGHT), "SFML-Networkingtest, Version " + vers,sf::Style::Default);
	win.setFramerateLimit(60);

	std::string fontName = FONT_BOLD;
	mainFont.loadFromFile("res\\fonts\\" + fontName);
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


//Returns for current stuff
sf::RenderWindow& cr::currWin() { return win; }
unsigned int cr::winWidth() { return win.getSize().x; }
unsigned int cr::winHeight() { return win.getSize().y; }
sf::Font& cr::currFont() { return mainFont; }
