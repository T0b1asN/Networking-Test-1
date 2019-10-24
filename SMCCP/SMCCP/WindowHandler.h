#pragma once
#include "SFML.h"

class WindowHandler
{
private:
	WindowHandler();

	sf::String test;

public:
	//needs to be declared, otherwise copies are possible
	WindowHandler(WindowHandler const&) = delete;
	void operator=(WindowHandler const&) = delete;

	~WindowHandler();

	static WindowHandler& get();

	void setTest(sf::String n) { test = n; }
	sf::String getTest() { return test; }
};

