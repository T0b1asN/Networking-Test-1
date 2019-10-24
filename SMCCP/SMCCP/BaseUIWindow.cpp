#include "BaseUIWindow.h"

BaseUIWindow::BaseUIWindow(sf::String title) : 
	window(sf::VideoMode(800,600), title, sf::Style::Close)
{
	window.setFramerateLimit(60);
	this->title = title;

	sf::Image icon;
	if (icon.loadFromFile("res\\AppIcon.png"))
		window.setIcon(626, 626, icon.getPixelsPtr());
	else
		own_log::AppendToLog("Icon could not be loaded!");
}

BaseUIWindow::BaseUIWindow(unsigned int width, unsigned int height, sf::String title) : 
	window(sf::VideoMode(width, height), title, sf::Style::Close)
{
	window.setFramerateLimit(60);
	this->title = title;

	sf::Image icon;
	if (icon.loadFromFile("res\\AppIcon.png"))
		window.setIcon(626, 626, icon.getPixelsPtr());
	else
		own_log::AppendToLog("Icon could not be loaded!");
}

BaseUIWindow::~BaseUIWindow()
{

}

int BaseUIWindow::run()
{
	return 0;
}

void BaseUIWindow::display()
{

}
