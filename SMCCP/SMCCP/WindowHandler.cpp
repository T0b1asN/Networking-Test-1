#include "WindowHandler.h"

WindowHandler::WindowHandler()
{
	
}

WindowHandler::~WindowHandler()
{
}

WindowHandler& WindowHandler::get()
{
	static WindowHandler instance;
	return instance;
}
