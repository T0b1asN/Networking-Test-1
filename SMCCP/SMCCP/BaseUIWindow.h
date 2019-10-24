#pragma once
#include "TextBox.h"
#include "CheckBox.h"
#include "OwnButton.h"

#include "SFML.h"

class BaseUIWindow
{
protected:
	sf::RenderWindow window;
	sf::String title;

	BaseUIWindow(sf::String title);
	BaseUIWindow(unsigned int width, unsigned int height, sf::String title);
	~BaseUIWindow();

	int run();

	void display();
};

