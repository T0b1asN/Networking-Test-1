//#define _CRT_SECURE_NO_WARNINGS

#include "SFML.h"
#include "Defines.h"

#include "Server.h"
#include "Client.h"
#include "curr.h"

#include "StartMenu.h"
#include "NamePrompt.h"
#include "PopUp.h"

#include <iostream>
#include <string>
#include <ctime>

#ifdef DISCORD_RICH_PRESENCE
#include "discord_register.h"
#include "discord_rpc.h"
#endif

//TODO have a loop that updates all present buttons, checkboxes, etc. which then call a callback when needed

void RunServer(std::string name, int port = 1234);
void RunClient(sf::IpAddress adress, unsigned int port = 1234);
std::string getCurrTime();

sf::Image icon;

void GraphicsSetup(unsigned int width, unsigned int height);

#ifdef DISCORD_RICH_PRESENCE
	void InitDiscord();
#endif

sf::RenderWindow win;
sf::Font mainFont;

std::vector<UIElement*> elementsVec;

int main()
{
	if (own_log::create() == 0)
	{
		debug::log("Created log file");
		own_log::append("Created log file");
	}

	if (!snd::LoadAllSounds())
		debug::log("Couldn't load all sounds");
	
#ifdef DISCORD_RICH_PRESENCE
	InitDiscord();
	cr::updateDiscordRPC("Testing");
#endif

	//TODO stuff like font loading is done for every new Window
	GraphicsSetup(500U, 200U);

#ifndef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // !NDEBUG

	StartMenu stMen;
	
	StartMenu::Result stMenRes = stMen.open();
	sf::IpAddress enteredIp = stMen.getIp();
	unsigned int port = stMen.getPort();
	debug::log("Port: " + std::to_string(port) + "; Ip: " + enteredIp.toString());

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
	default:
		own_log::append("Wrong return from StartMenu!");
		debug::log("Wrong return fromm StartMenu!");
		//TODO stop only if in debug (new method in own_log)
		system("pause");
		//TODO error box (if in release)
		break;
	}
	
#ifdef DISCORD_RICH_PRESENCE
	Discord_Shutdown();
#endif
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
		own_log::append("Icon could not be loaded!");
	std::string fontName = FONT_NORM;
	mainFont.loadFromFile("res\\fonts\\" + fontName);

	sf::sleep(sf::milliseconds(50));

	input::setFocus(&win);
}

void RunServer(std::string name, int port)
{
	Server server(name, false, port, 10);
	int setupCode = server.setup();
	if (setupCode != 0)
	{
		own_log::append("Error " + std::to_string(setupCode) + " while setting up the server");
		return;
	}
	server.connectToClient();
	server.Run();
}

void RunClient(sf::IpAddress adress, unsigned int port)
{
	Client client(false, port, adress);
	if(client.Setup() == 2)
		return;
	client.Run();
}

std::string getCurrTime()
{
	time_t t = time(0);   // get time now
	struct tm now;
	localtime_s(&now, &t);
	std::string out;
	out += std::to_string(now.tm_mday) + 
		"/" + std::to_string(now.tm_mon + 1) + 
		"/" + std::to_string(now.tm_year + 1900) + 
		" - " + std::to_string(now.tm_hour) + 
		":" + std::to_string(now.tm_min);
	return out;
}

//Returns for current stuff
sf::RenderWindow& cr::currWin() { return win; }
unsigned int cr::winWidth() { return win.getSize().x; }
unsigned int cr::winHeight() { return win.getSize().y; }
sf::Font& cr::currFont() { return mainFont; }

void cr::addUIElement(UIElement* element)
{
	std::cout << "Add: " << element << std::endl;
	elementsVec.push_back(element);
}

void cr::deleteUIElement(UIElement * element)
{
	std::cout << "Delete: " << element << std::endl;
	for each (UIElement* i in elementsVec)
	{
		std::cout << i << std::endl;
	}

	if (elementsVec.empty())
		return;
	auto p = std::find(elementsVec.begin(), elementsVec.end(), element);
	if (p != elementsVec.end())
		elementsVec.erase(p);

	for each (UIElement* i in elementsVec)
	{
		std::cout << i << std::endl;
	}
}

void cr::updateUIElements()
{
	std::for_each(elementsVec.begin(), elementsVec.end(), [](UIElement * el)
	{
		el->update();
	});
}

std::vector<UIElement*> cr::elements()
{
	return elementsVec;
}

//DiscordRPC stuff
#ifdef DISCORD_RICH_PRESENCE
void cr::updateDiscordRPC(std::string details)
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.details = details.c_str();
	discordPresence.startTimestamp = own_log::getTimeStamp();
	discordPresence.largeImageKey = "appicon_png";
	Discord_UpdatePresence(&discordPresence);
}

void discord_ready(const DiscordUser* request)
{
	std::cout << "Discord username: " << request->username << std::endl;
}

void discord_error(int error, const char* msg)
{
	std::cout << "Error " << std::to_string(error) << ": " << msg << std::endl;
}

void discord_disconnect(int error, const char* msg)
{
	std::cout << "Disconnect: (Error " << std::to_string(error) << "); " << msg << std::endl;
}

void InitDiscord()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready = discord_ready;
	handlers.errored = discord_error;
	handlers.disconnected = discord_disconnect;

	// Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
	Discord_Initialize(DISCORD_CLIENT_ID, &handlers, 0, "");
	debug::pushMsgToCommandIfDebug("Discord Rich Presence enabled!");
}
#endif
